/*
 * Warrior.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: dario
 */

#include "Warrior.h"
#include "../Utils/Log.h"

const string TAG = "Warrior";

Warrior::Warrior(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: MobileModel(id,nombre, posicion, ancho_base, alto_base){}

Warrior::Warrior() {}

Warrior::~Warrior() {}

void Warrior::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

void Warrior::receiveInteraction(Building* entity) {
	Log().Get(TAG) << "Warrior receive interaction from building -> Do nothing";
}
