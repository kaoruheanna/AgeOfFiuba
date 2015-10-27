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
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		map<string, MobileModel*>::iterator usuario;
		for (usuario = this->escenario->usuarios.begin(); usuario != this->escenario->usuarios.end(); ++usuario){
			mensajeroReal->actualizaPersonaje(usuario->second);
		}
	}
}

void ServerGameController::moverProtagonista(MobileModel* model) {
	// TODO volver a hacer sincronico
	MobileModel* protagonista = this->escenario->getUserModel(model->getUsername());
	if(protagonista == NULL){
		Log().Get(TAG, logERROR) << "El personaje: " << model->getUsername() << " no existe";
		return;
	}

	SDL_Point origen = protagonista->getPosicion();
	SDL_Point destino = {model->getDestinationX(),model->getDestinationY()};
//	queue <SDL_Point> camino = this->escenario->getPath(origen,{model->getDestinationX(),model->getDestinationY()});
	queue <SDL_Point> camino = this->escenario->getCaminoForEntity(origen,destino,protagonista);
	protagonista->setPath(camino);
	Log().Get(TAG, logDEBUG) << "El personaje: " << model->getUsername() << " se mueve al: " << model->getDestinationX() << " , " << model->getDestinationY();
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
	MobileModel* user = this->escenario->getUserModel(username);
	if(user != NULL){
		user->setActive(true);
	}
}

void ServerGameController::setUserInactive(char* username) {
	MobileModel* user = this->escenario->getUserModel(username);
	if(user != NULL){
		user->setActive(false);
	}
}
