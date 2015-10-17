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
	entidadAparecida = NULL;
	moverPersonajeAlPunto.first = -1;
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

void ServerGameController::loopEscenario() {
	if (this->moverPersonajeAlPunto.first != -1) {
		this->escenario->getProtagonista()
				->setDestination(
					moverPersonajeAlPunto.first,
					moverPersonajeAlPunto.second
				);
		this->moverPersonajeAlPunto.first = -1;
	}

	this->escenario->loop();

	if(debeActualizarPersonaje) {
		debeActualizarPersonaje = false;
		this->actualizarProtagonista();
	}

	if (this->entidadAparecida) {
		list<Mensajero*>::iterator mensajero;
		for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
			Mensajero* mensajeroReal = (*mensajero);
			mensajeroReal->apareceRecurso((Resource*)this->entidadAparecida);
		}
		delete this->entidadAparecida;
		this->entidadAparecida = NULL;
	}
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
	this->moverPersonajeAlPunto.first = model->getDestinationX();
	this->moverPersonajeAlPunto.second = model->getDestinationY();
}

void ServerGameController::addMensajero(Mensajero* mensajero) {
	this->mensajeros.push_back(mensajero);
	if(escenario->inicializacionCorrecta) {
		mensajero->configEscenario(this->config->getPath());
	}
}

void ServerGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}

void ServerGameController::actualizaPersonaje(MobileModel* entity){
	this->debeActualizarPersonaje = true;
}

void ServerGameController::apareceEntidad(Entity* recurso) {
	this->entidadAparecida = new Entity(*recurso);

}

void ServerGameController::desapareceEntidad(Entity* recurso) {
	/*
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->desapareceRecurso((Resource*)recurso);
	}*/
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
