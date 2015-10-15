/*
 * ServerGameController.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "ServerGameController.h"
#include "../Utils/Log.h"

static const string TAG = "ServerGameController";

ServerGameController::ServerGameController(GameConfiguration *config) : config(config), escenario() {}

ServerGameController::~ServerGameController() {}

void ServerGameController::play() {
	// Crear modelos a partir de la configuracion
	this->escenario = new Escenario(this->config->getEscenario(), this->config->getTipos());
	if(!this->escenario->inicializacionCorrecta){
		delete this->escenario;
		delete this->config;
		// Cargar configuracion default
		this->config = new GameConfiguration();
		this->escenario = new Escenario(this->config->getEscenario(), this->config->getTipos());
		if(!this->escenario->inicializacionCorrecta){
			Log().Get(TAG, logERROR)
					<< "La configuracion default es incorrecta.";
		}
	}
}

bool ServerGameController::inicializado() {
	return this->escenario->inicializacionCorrecta;
}

GameConfiguration * ServerGameController::getConfig() {
	return this->config;
}
