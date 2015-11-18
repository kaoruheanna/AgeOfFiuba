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

void MensajeroLocal::comenzoPartida() {
	this->cliente->comenzoPartida();
}


void MensajeroLocal::addClient(MensajeroCliente* client) {
	this->cliente = client;
	this->server->addMensajero(this);
}


void MensajeroLocal::moverEntidad(MobileModel* entity, string username){
	this->server->moverEntidad(entity, username);
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

void MensajeroLocal::actualizarEntidad(Entity* entity) {
	this->cliente->actualizarEntidad(new Entity(*entity));
}

void MensajeroLocal::actualizaPersonaje(MobileModel* entity){
	this->cliente->actualizaPersonaje(entity);
}

void MensajeroLocal::cambioUsuario(User* user) {
	this->cliente->cambioUsuario(user);
}

void MensajeroLocal::actualizarRecursos(User* auxUser) {
	this->cliente->actualizarRecursos(auxUser);
}

// Metodos Cliente -> Servidor
void MensajeroLocal::loguearse(char* nombre) {
	this->server->loguearse(nombre);
}

void MensajeroLocal::construir(Entity *entity) {
	this->server->construir(new Entity(*entity));
}



