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
	moverPersonajeAlPunto= NULL;
}

ServerGameController::~ServerGameController() {}


void escenarioInicializado(list<Mensajero*> mensajeros,Escenario* escenario, const string path) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->configEscenario(path);
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
	if (this->moverPersonajeAlPunto != NULL) {
		Log().Get(TAG) << "Personaje se mueve al " << this->moverPersonajeAlPunto->first << " , " << this->moverPersonajeAlPunto->second;
		//Directo
		/*this->escenario->getProtagonista()->setDestination(
				moverPersonajeAlPunto->first,moverPersonajeAlPunto->second);*/

		//Con caminno minimo

		SDL_Point origen = this->escenario->getProtagonista()->getPosicion();
		queue <SDL_Point> camino = this->escenario->getPath(origen,{moverPersonajeAlPunto->first,moverPersonajeAlPunto->second});
		this->escenario->getProtagonista()->setPath(camino);

		delete this->moverPersonajeAlPunto;
		this->moverPersonajeAlPunto = NULL;
	}
}

void ServerGameController::enviarEventos() {
	if(debeActualizarPersonaje) {
		debeActualizarPersonaje = false;
		this->actualizarProtagonista();
	}

	list<Entity*>::iterator entidad;
	for (entidad = recursos.begin(); entidad != recursos.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		list<Mensajero*>::iterator mensajero;
		for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
			Mensajero* mensajeroReal = (*mensajero);
			mensajeroReal->apareceRecurso((Resource*)entidadReal);
		}
	}
	recursos.clear();

	list<Entity*>::iterator entidadEliminada;
	for (entidadEliminada = recursosEliminados.begin(); entidadEliminada != recursosEliminados.end(); ++entidadEliminada){
		Entity* entidadReal = (*entidadEliminada);
		list<Mensajero*>::iterator mensajero;
		for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
			Mensajero* mensajeroReal = (*mensajero);
			mensajeroReal->desapareceRecurso((Resource*)entidadReal);
		}
	}
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
		mensajeroReal->actualizaPersonaje(this->escenario->getProtagonista());
	}
}

void ServerGameController::moverProtagonista(MobileModel* model) {
	if(this->moverPersonajeAlPunto) {
		delete this->moverPersonajeAlPunto;
	}
	this->moverPersonajeAlPunto = new Posicion();
	this->moverPersonajeAlPunto->first = model->getDestinationX();
	this->moverPersonajeAlPunto->second = model->getDestinationY();
	Log().Get(TAG) << "Callbac se mueve al " << this->moverPersonajeAlPunto->first << " , " << this->moverPersonajeAlPunto->second;
}

void ServerGameController::addMensajero(Mensajero* mensajero) {
	this->mensajeros.push_back(mensajero);
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
	this->recursos.push_back(recurso);
}

void ServerGameController::desapareceEntidad(Entity* recurso) {
	this->recursosEliminados.push_back(recurso);
}

// TODO Implementar el manejo de usuarios

bool ServerGameController::userExists(char* username) {
	return false;
}

bool ServerGameController::userActive(char* username) {
	return false;
}

void ServerGameController::addUser(char* username) {

}
