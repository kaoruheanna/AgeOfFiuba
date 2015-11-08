/*
 * ServerGameController.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "ServerGameController.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

static const string TAG = "ServerGameController";

ServerGameController::ServerGameController(GameConfiguration *config) :  config(config) {
	escenario = NULL;
	debeActualizarPersonaje = false;
	moverPersonajeAlPunto = NULL;
}

ServerGameController::~ServerGameController() {}


void escenarioInicializado(list<Mensajero*> mensajeros,Escenario* escenario, const string path) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->configEscenario(path);
	}
}

void aparecenRecursos(Mensajero* mensajero,list<Entity*> recursos) {
	list<Entity*>::iterator entidad;
	for (entidad = recursos.begin(); entidad != recursos.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		Log().Get(TAG) << entidadReal->getNombre();
		mensajero->apareceRecurso((Resource*)entidadReal);
	}
}

void ServerGameController::init() {
	// Crear modelos a partir de la configuracion
	this->escenario = new Escenario(this->config->getEscenario(),
			this->config->getTipos());
	if (!this->escenario->inicializacionCorrecta) {
		delete this->escenario;
		delete this->config;
		// Cargar configuracion default
		this->config = new GameConfiguration();
		this->escenario = new Escenario(this->config->getEscenario(),
				this->config->getTipos());
		if (!this->escenario->inicializacionCorrecta) {
			Log().Get(TAG, logERROR)
					<< "La configuracion default es incorrecta.";
		}
	}
	this->escenario->delegate = this;
	escenarioInicializado(this->mensajeros,this->escenario,this->config->getPath());
}

void ServerGameController::play() {

	if (!this->inicializado()) {
		init();
	}
	while( true ) {
		this->loopEscenario();
		this->sleep();
	}
}

void ServerGameController::obtenerEventos() {

}

void ServerGameController::enviarEventos() {
	this->actualizarProtagonista();

	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		aparecenRecursos(mensajeroReal,this->recursosAgregados);
	}
	this->recursosAgregados.clear();

	list<Entity*>::iterator entidadEliminada;
	for (entidadEliminada = recursosEliminados.begin(); entidadEliminada != recursosEliminados.end(); ++entidadEliminada){
		Entity* entidadReal = (*entidadEliminada);
		recursosAgregados.remove(entidadReal);
		list<Mensajero*>::iterator mensajero;
		for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
			Mensajero* mensajeroReal = (*mensajero);
			mensajeroReal->desapareceRecurso((Resource*)entidadReal);
		}
	}
	recursosEliminados.clear();

	for(auto nuevoMensajero : this->mensajerosAgregados) {
		this->mensajeros.push_back(nuevoMensajero);
		aparecenRecursos(nuevoMensajero,this->escenario->getListaRecursos());
	}
	this->mensajerosAgregados.clear();
}

void ServerGameController::loopEscenario() {
	this->obtenerEventos();
	this->escenario->loop();
	this->enviarEventos();
}

bool ServerGameController::inicializado() {
	return this->escenario && this->escenario->inicializacionCorrecta;
}

GameConfiguration * ServerGameController::getConfig() {
	return this->config;
}

void ServerGameController::actualizarProtagonista(){
	list<MobileModel*> mobileModels = this->escenario->getMobileModels();
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		list<MobileModel*>::iterator mobileModel;
		for (mobileModel = mobileModels.begin(); mobileModel != mobileModels.end(); ++mobileModel){
			mensajeroReal->actualizaPersonaje(*mobileModel);
		}
	}
}

void ServerGameController::moverEntidad(MobileModel* newModel, string username) {
	// TODO volver a hacer sincronico
	MobileModel* oldModel = this->getMobileModelForUser(newModel->getId(), username);
	if(oldModel == NULL){
		// TODO mandar error de que no le pertenece la entidad
		return;
	}

	SDL_Point origen = oldModel->getPosicion();
	SDL_Point destino = {newModel->getDestinationX(),newModel->getDestinationY()};
	queue <SDL_Point> camino = this->escenario->getCaminoForMobileModel(origen,destino,oldModel);
	oldModel->setPath(camino);
	Log().Get(TAG, logDEBUG) << "El personaje: " << oldModel->getUsername() << " se mueve al: " << oldModel->getDestinationX() << " , " << oldModel->getDestinationY() << " camino: " << camino.size();
}

void ServerGameController::interactuar(int selectedEntityId, int targetEntityId) {
	Entity* selectedEntity = this->escenario->entidadConId(selectedEntityId);
	Entity* targetEntity = this->escenario->entidadConId(targetEntityId);
	if(!selectedEntity) {
		Log().Get(TAG) << "No se encontro entidad con id " << selectedEntityId;
		return;
	}
	if(!targetEntity) {
		Log().Get(TAG) << "No se encontro entidad con id " << targetEntityId;
		return;
	}

	Log().Get(TAG) << "Interactuar en el server " << selectedEntity->getNombre() << " " << targetEntity->getNombre();
	selectedEntity->interact(targetEntity);
}

void ServerGameController::addMensajero(Mensajero* mensajero) {
	this->mensajerosAgregados.push_back(mensajero);
	if(escenario->inicializacionCorrecta) {
		mensajero->configEscenario(this->config->getPath());
	}
}

void ServerGameController::removeMensajero(Mensajero *mensajero) {
	this->mensajeros.remove(mensajero);
}

void ServerGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}

void ServerGameController::actualizaPersonaje(MobileModel* entity){
	this->debeActualizarPersonaje = true;
}

void ServerGameController::apareceEntidad(Entity* recurso) {
	this->recursosAgregados.push_back(recurso);
}

void ServerGameController::desapareceEntidad(Entity* recurso) {
	this->recursosEliminados.push_back(recurso);
}

// TODO Implementar el manejo de usuarios

bool ServerGameController::userExists(char* username) {
	return (this->escenario->getUserModel(username) != NULL);
}

bool ServerGameController::userActive(char* username) {
	if(!this->userExists(username)){
		return false;
	}
	return this->escenario->getUserModel(username)->isActive();
}

void ServerGameController::addUser(char* username) {
	this->escenario->addUser(username);
}

void ServerGameController::setUserActive(char* username) {
	User* user = this->getUserByName(username);
	if(user != NULL){
		user->setActive(true);
	}
	// TODO logica de mas para mantener un "protagonista" por equipo
	MobileModel* userModel = this->escenario->getUserModel(username);
	if(userModel != NULL){
		userModel->setActive(true);
	}
}

void ServerGameController::setUserInactive(char* username) {
	User* user = this->getUserByName(username);
	if(user != NULL){
		user->setActive(false);
	}
	// TODO logica de mas para mantener un "protagonista" por equipo
	MobileModel* userModel = this->escenario->getUserModel(username);
	if(userModel != NULL){
		userModel->setActive(false);
	}
}

bool ServerGameController::teamAvailable() {
	return (this->escenario->getTeams().size() > this->usuarios.size());
}

User* ServerGameController::getUserByName(string username) {
	User* found = NULL;
	list<User*>::iterator userIt = this->usuarios.begin();
	while(found == NULL && (userIt != this->usuarios.end())){
		if((*userIt)->getName().compare(username) == 0){
			found = (*userIt);
		}
		++userIt;
	}
	return found;
}

Entity* ServerGameController::getEntityForUser(int entityId, string username) {
	User* user = this->getUserByName(username);
	if(user == NULL){
		// El usuario no existe => no tiene entidades
		return NULL;
	}
	Entity* entity = this->escenario->entidadConId(entityId);
	if(entity == NULL){
		// La entidad no existe
		return NULL;
	}
	if(entity->getTeam() != user->getTeam()){
		// La entidad no le pertenece al usuario => no puede darle ordenes
		return NULL;
	}
	return entity;
}

MobileModel* ServerGameController::getMobileModelForUser(int entityId, string username) {
	Entity* entity = this->getEntityForUser(entityId, username);
	if(entity == NULL){
		// La entidad no existe / no le pertenece al usuario
		return NULL;
	}
	if(entity->getClass() != MOBILE_MODEL){
		// La entidad no es un mobile model
		return NULL;
	}
	return (MobileModel*) entity;
}

int ServerGameController::userLogin(char* username) {
	User* user = this->getUserByName(username);
	if(user != NULL){
		if(user->isActive()){
			// Error de logueo => Nombre tomado
			return -1;
		} else {
			// Ya se habia logueado antes
			return 0;
		}
	}
	// Crear nuevo usuario
	list<Team> teams = this->escenario->getTeams();
	if(teams.size() <= this->usuarios.size()){
		// Error de logueo => No hay mas equipos disponibles
		return -2;
	}
	user = new User(string(username));
	// Obtener Team en el indice usuarios.size()
	list<Team>::iterator teamIt = teams.begin();
	int falta = this->usuarios.size();
	while(falta != 0){
		++teamIt;
		falta--;
	}
	user->setTeam(*teamIt);
	this->usuarios.push_back(user);
	// TODO logica de mas para mantener un "protagonista" por equipo
	list<Entity*> entidades = this->escenario->getListaEntidades();
	list<Entity*>::iterator entidadIt = entidades.begin();
	for(; entidadIt != entidades.end(); ++entidadIt){
		Entity* entidad = (*entidadIt);
		if(entidad->getClass() == MOBILE_MODEL){
			MobileModel* personaje = (MobileModel*)entidad;
			if(personaje->getTeam() == user->getTeam()){
				personaje->setUsername(user->getName());
				this->escenario->usuarios.insert(pair<string, MobileModel*>(username, personaje));
			}
		}
	}
	printf("Servidor - Nuevo Logueo con equipo: %i\n", user->getTeam());
	this->actualizarProtagonista();
	return 0;
}
