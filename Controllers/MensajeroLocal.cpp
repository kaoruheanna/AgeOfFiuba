/*
 * Mensajero.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "MensajeroLocal.h"

MensajeroLocal::MensajeroLocal(ServerGameController* server) : Mensajero() {
	this->server = server;
	this->client = NULL;
}

MensajeroLocal::~MensajeroLocal() {}

GameConfiguration* MensajeroLocal::obtenerConfiguracion(){
	return this->server->getConfig();
}

bool MensajeroLocal::inicializado() {
	return this->server->inicializado();
}

Escenario* MensajeroLocal::obtenerEscenario() {
	return this->server->escenario;
}
