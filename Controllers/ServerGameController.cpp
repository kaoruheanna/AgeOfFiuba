/*
 * ServerGameController.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "ServerGameController.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "../Utils/EscenarioSingleton.h"

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

void actualizarEntidades(Mensajero* mensajero,list<Entity*> entities) {
	list<Entity*>::iterator entidad;
	for (entidad = entities.begin(); entidad != entities.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		mensajero->actualizarEntidad(entidadReal);
	}
}

void ServerGameController::init() {
	// Crear modelos a partir de la configuracion
	// Creo el escenario y se lo paso al EscenarioSingleton
	this->escenario = EscenarioSingleton::get(new Escenario(this->config->getEscenario(),
			this->config->getTipos()));
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
		actualizarEntidades(mensajeroReal,this->entidadesActualizadas);
	}
	this->entidadesActualizadas.clear();

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
		aparecenRecursos(nuevoMensajero,this->escenario->getListaEntidades());
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
	newModel->stopInteracting();
	this->escenario->entidadesInteractuando.remove(newModel);
	// TODO volver a hacer sincronico
	MobileModel* oldModel = this->getMobileModelForUser(newModel->getId(), username);
	if(oldModel == NULL){
		// TODO mandar error de que no le pertenece la entidad
		return;
	}
	SDL_Point destino = {newModel->getDestinationX(),newModel->getDestinationY()};
	this->escenario->moveEntityToPos(oldModel,destino);
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

	//if(selectedEntity->shouldInteractWith(targetEntity)) {
		Log().Get(TAG) << "Agrego a Interactuar en el server " << selectedEntity->getNombre() << " " << targetEntity->getNombre();
		selectedEntity->interact(targetEntity);
		this->escenario->entidadesInteractuando.push_back(selectedEntity);
	//}

}

void ServerGameController::addMensajero(Mensajero* mensajero) {
	this->mensajerosAgregados.push_back(mensajero);
	if(escenario->inicializacionCorrecta) {
		mensajero->configEscenario(this->config->getPath());
		printf("Mandar usuarios");
		list<User*>::iterator usuario;
		for (usuario = usuarios.begin(); usuario != usuarios.end(); ++usuario){
			mensajero->cambioUsuario(*usuario);
		}
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

void ServerGameController::actualizaEntidad(Entity* recurso) {
	this->entidadesActualizadas.push_back(recurso);
}

void ServerGameController::desapareceEntidad(Entity* recurso) {
	this->recursosEliminados.push_back(recurso);
}

// TODO Implementar el manejo de usuarios

void ServerGameController::setUserActive(char* username) {
	User* user = this->getUserByName(username);
	if(user != NULL){
		user->setActive(true);
	}
}

void ServerGameController::setUserInactive(char* username) {
	User* user = this->getUserByName(username);
	if(user != NULL){
		user->setActive(false);
	}
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
	return 0;
}

void ServerGameController::mandarUsuarios() {
	list<MobileModel*> mobileModels = this->escenario->getMobileModels();
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		list<User*>::iterator usuario;
		for (usuario = usuarios.begin(); usuario != usuarios.end(); ++usuario){
			mensajeroReal->cambioUsuario(*usuario);
		}
	}
}
