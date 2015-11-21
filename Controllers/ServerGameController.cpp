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
	comenzoPartida = false;
}

ServerGameController::~ServerGameController() {}


void escenarioInicializado(list<Mensajero*> mensajeros,Escenario* escenario, const string path) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->configEscenario(path);
	}
}

void actualizarEntidadesEstaticas(Mensajero* mensajero,list<Entity*> entities) {
	list<Entity*>::iterator entidad;
	for (entidad = entities.begin(); entidad != entities.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		if (entidadReal->getClass() != MOBILE_MODEL){
			mensajero->actualizarEntidad(entidadReal);
		}
	}
}

void agregarEntidadesEstaticas(Mensajero* mensajero,list<Entity*> entities) {
	list<Entity*>::iterator entidad;
	for (entidad = entities.begin(); entidad != entities.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		//no mando los mobile models porque se mandan aparte
		if (entidadReal->getClass() != MOBILE_MODEL){
			mensajero->actualizarEntidad(entidadReal);
		}
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
	while( !comenzoPartida ){
		this->resolverMensajeros();
		this->sleep();
	}
	this->resolverMensajeros();
	this->enviarEventos();
	this->enviarComienzo();

	while( true ) {
		this->resolverMensajeros();
		this->loopEscenario();
		this->sleep();
	}
}

void ServerGameController::enviarComienzo() {
	printf("Enviado comienzo a jugadores: %i\n", mensajeros.size());
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->comenzoPartida();
	}
}

void ServerGameController::obtenerEventos() {

}

void ServerGameController::enviarEventos() {
	this->actualizarProtagonista();

	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		actualizarEntidadesEstaticas(mensajeroReal,this->entidadesEstaticasActualizadas);
		agregarEntidadesEstaticas(mensajeroReal,this->escenario->entidadesAgregadas);
	}
	this->entidadesEstaticasActualizadas.clear();
	this->escenario->entidadesAgregadas.clear();

/*
 * Hay q ver si esto va (no estoy seguro)
 */
//	list<Entity*>::iterator entidadEliminada;
//	for (entidadEliminada = recursosEliminados.begin(); entidadEliminada != recursosEliminados.end(); ++entidadEliminada){
//		Entity* entidadReal = (*entidadEliminada);
//		recursosAgregados.remove(entidadReal);
//		list<Mensajero*>::iterator mensajero;
//		for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
//			Mensajero* mensajeroReal = (*mensajero);
//			mensajeroReal->desapareceRecurso((Resource*)entidadReal);
//		}
//	}
//	recursosEliminados.clear();

	if(this->debeActualizarUsuarios){
		this->debeActualizarUsuarios = false;
		this->mandarUsuarios();
	}
}

/**
 * Metodo para agregar nuevos mensajeros y mantenerlos a todos pingeados
 * Si se saca de cualquier while genera los problemas de desconexion
 */
void ServerGameController::resolverMensajeros() {
	// Agregar nuevos mensajeros
	for(auto nuevoMensajero : this->mensajerosAgregados) {
		this->mensajeros.push_back(nuevoMensajero);
	}
	this->mensajerosAgregados.clear();
	// Pingear a todos para que no se desconecten
	for(auto mensajero : this->mensajeros){
		mensajero->ping();
	}
}

void ServerGameController::setResourcesGathered() {
	list<Entity*> entidades = this->escenario->getListaEntidades();
	list<Entity*>::iterator iterador;
	for (iterador = entidades.begin(); iterador != entidades.end();++iterador) {
		Entity* entity = *iterador;
		//Preguntar si recolecto o no
		if (entity->hasGatheredResources()) {
			User* user = this->getUserByTeam(entity->getTeam());
			//			if (user && user->isActive()){
			if (user) {
				user->comida += entity->foodGathered;
				user->madera += entity->woodGathered;
				user->piedra += entity->stoneGathered;
				user->oro += entity->goldGathered;
				entity->resetResourcesGathered();
				this->debeActualizarUsuarios = true;
			}
		}
	}
}

void ServerGameController::loopEscenario() {
	this->obtenerEventos();
	this->escenario->loop();
	this->setResourcesGathered();
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
	oldModel->stopInteracting();
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

	Log().Get(TAG) << "Agrego a Interactuar en el server " << selectedEntity->getNombre() << " " << targetEntity->getNombre();
	Log().Get(TAG) << "Vida Inicial " << targetEntity->getLife();

	if(selectedEntity->getClass()==MOBILE_MODEL) {
		SDL_Point point = this->escenario->mundo->getPuntoMasCercanoADistancia(selectedEntity,targetEntity,selectedEntity->getAlcance());
		SDL_Point destino = this->escenario->mundo->getCenteredPositionForTile(point);
		this->escenario->moveEntityToPos((MobileModel*)selectedEntity,destino);
	}
	selectedEntity->interact(targetEntity);

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
	comenzoPartida = this->escenario->getTeams().size() == this->usuarios.size();
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

void ServerGameController::actualizaEntidadEstatica(Entity* entidad) {
	if (entidad->esMobileModel()){
		return;
	}

	//evito guardarla dos veces
	for(auto savedEntity : this->entidadesEstaticasActualizadas) {
		if (savedEntity->getId() == entidad->getId()){
			return;
		}
	}

	this->entidadesEstaticasActualizadas.push_back(entidad);
}

//ver si esto va
void ServerGameController::desapareceEntidad(Entity* entidad) {
//	this->recursosEliminados.push_back(recurso);
}

void ServerGameController::equipoPerdio(Team equipo) {
	User* teamUser = this->getUserByTeam(equipo);
	if(teamUser != NULL){
		teamUser->perdio = true;
		this->debeActualizarUsuarios = true;
	}
}

void ServerGameController::equipoGano(Team equipo) {
	User* teamUser = this->getUserByTeam(equipo);
	if(teamUser != NULL){
		teamUser->gano = true;
		this->debeActualizarUsuarios = true;
	}
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
		if(this->escenario->tipo != NULL){
			this->escenario->tipo->equipoInactivo(user->getTeam());
		}
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

User* ServerGameController::getUserByTeam(Team team) {
	User* found = NULL;
	list<User*>::iterator userIt = this->usuarios.begin();
	while(found == NULL && (userIt != this->usuarios.end())){
		if((*userIt)->getTeam() == team){
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
	if(teams.size() <= this->usuarios.size() || this->comenzoPartida){
		// Error de logueo => No hay mas equipos disponibles
		Log().Get(TAG) << "Cantidad de equipos " << teams.size() << " usuarios" << this->usuarios.size() << " comenzo partida " << this->comenzoPartida;
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
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		list<User*>::iterator usuario;
		for (usuario = usuarios.begin(); usuario != usuarios.end(); ++usuario){
			mensajeroReal->cambioUsuario(*usuario);
		}
	}
}

void ServerGameController::construir(Entity* tempEntity){
	LogicPosition logicPosition = LogicPosition(tempEntity->getPosicion().x,tempEntity->getPosicion().y);
	this->escenario->crearYAgregarNuevaEntidad(tempEntity->getNombre(),logicPosition,tempEntity->getTeamString());
}
