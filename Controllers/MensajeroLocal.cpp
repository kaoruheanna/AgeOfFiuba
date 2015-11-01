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

void MensajeroLocal::configEscenario(const string path) {
	this->cliente->configEscenario(path);
}


void MensajeroLocal::addClient(MensajeroCliente* client) {
	this->cliente = client;
	this->server->addMensajero(this);
}

void MensajeroLocal::moverProtagonista(MobileModel* model) {
	this->server->moverProtagonista(model);
}

void MensajeroLocal::interactuar(int selectedEntityId, int targetEntityId) {
	this->server->interactuar(selectedEntityId, targetEntityId) ;
}

void MensajeroLocal::apareceRecurso(Resource* recurso){
	this->cliente->apareceRecurso(new Resource(*recurso));
}

void MensajeroLocal::desapareceRecurso(Resource* recurso){
	this->cliente->desapareceRecurso(new Resource(*recurso));
}

void MensajeroLocal::actualizaPersonaje(MobileModel* entity){
	this->cliente->actualizaPersonaje(entity);
}
