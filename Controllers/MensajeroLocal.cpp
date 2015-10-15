/*
 * Mensajero.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "MensajeroLocal.h"

MensajeroLocal::MensajeroLocal() : Mensajero() {
	this->server = NULL;
	this->cliente = NULL;
}

MensajeroLocal::MensajeroLocal(ServerGameController* server) : Mensajero() {
	this->server = server;
	this->cliente = NULL;
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

void MensajeroLocal::addClient(MensajeroCliente* client) {
	this->cliente = client;
	this->server->addMensajero(this);
}

void MensajeroLocal::moverProtagonista(SDL_Point point) {
	this->server->moverProtagonista(point);
}

void MensajeroLocal::apareceRecurso(Resource* recurso){
	this->cliente->apareceRecurso(recurso);
}

void MensajeroLocal::desapareceRecurso(Resource recurso){
	this->cliente->desapareceRecurso(recurso);
}

void MensajeroLocal::actualizaPersonaje(MobileModel* entity){
	this->cliente->actualizaPersonaje(entity);
}
