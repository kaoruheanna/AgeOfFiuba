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
: Entity(id,nombre, posicion, ancho_base, alto_base){}

Building::Building() : Entity(0,"", {0, 0}, 1, 1) {
	this->life = 1000;
}

Building::~Building() {}

void Building::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

void Building::receiveInteraction(Warrior* entity) {
	if(!this->estaViva()) {
		return;
	}

	//Escenario* escenario = EscenarioSingleton::get();

	// TODO Moverse hasta el lugar intermadio
	//SDL_Point destino = {this->posicion.x,this->posicion.y};
	//escenario->moveEntityToPos(entity,destino);

	if (!entity->canReach(this)) {
		return;
	}
	this->life = this->life - entity->getPoderAtaque();
	Log().Get(TAG) << "Building receive interaction from Warrior vida: " << this->life;
}
