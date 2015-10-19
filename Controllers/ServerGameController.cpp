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
}

ServerGameController::~ServerGameController() {}


void escenarioInicializado(list<Mensajero*> mensajeros,Escenario* escenario) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->configEscenario(CONFIG_CUSTOM);
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
	escenarioInicializado(this->mensajeros,this->escenario);
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
	this->escenario->loop();
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

void ServerGameController::moverProtagonista(SDL_Point point) {
	SDL_Point origen = this->escenario->getProtagonista()->getPosicion();
	queue <SDL_Point> camino = this->escenario->getPath(origen,{point.x,point.y});
	this->escenario->getProtagonista()->setPath(camino);

	//this->escenario->getProtagonista()->setDestination(point.x,point.y);
}

void ServerGameController::addMensajero(Mensajero* mensajero) {
	this->mensajeros.push_back(mensajero);
	if(escenario->inicializacionCorrecta) {
		mensajero->configEscenario(CONFIG_CUSTOM);
	}
}

void ServerGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}

void ServerGameController::actualizaPersonaje(MobileModel* entity){
	this->actualizarProtagonista();
}

void ServerGameController::apareceEntidad(Entity* recurso) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->apareceRecurso((Resource*)recurso);
	}
}

void ServerGameController::desapareceEntidad(Entity* recurso) {
	list<Mensajero*>::iterator mensajero;
	for (mensajero = mensajeros.begin(); mensajero != mensajeros.end(); ++mensajero){
		Mensajero* mensajeroReal = (*mensajero);
		mensajeroReal->desapareceRecurso((Resource*)recurso);
	}
}

