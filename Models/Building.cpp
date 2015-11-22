/*
 * Building.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#include "Building.h"
#include "../Utils/Log.h"
#include "../Utils/EscenarioSingleton.h"

const string  TAG = "Building";

Building::Building(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: Entity(id,nombre, posicion, ancho_base, alto_base){
	this->life = 100;
	this->asesino = TEAM_NEUTRAL;
}

Building::Building() : Entity(0,"", {0, 0}, 1, 1) {
	this->life = 100;
	this->asesino = TEAM_NEUTRAL;
}

Building::~Building() {}

void Building::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

void Building::receiveInteraction(Warrior* entity) {
	if(!this->estaViva()) {
		return;
	}

	this->life -= this->vidaDescontada(entity);
	if((this->life % 100) == 0) {
		Log().Get(TAG) << "Building receive interaction from Warrior vida: " << this->life;
	}
	if(!this->estaViva()){
		this->asesino = entity->getTeam();
	}
}

void Building::receiveInteraction(Worker* worker){
	if (!this->estaViva()){
		return;
	}

	if (worker->getTeam() != this->getTeam()){
		return;
	}

	int progreso = this->getProgresoConstruccion();
	progreso += worker->getPoderCosecha();
	Log().Get(TAG) << "Building receive interaction from worker progreso: " << progreso;
	this->setProgresoConstruccion(progreso);

	if (this->esProgresoCompleto()){
		worker->stopInteracting();
	}
}
