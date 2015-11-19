/*
 * Worker.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#include "Worker.h"
#include "../Utils/Log.h"
const string  TAG = "Worker";

Worker::Worker(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: MobileModel(id,nombre, posicion, ancho_base, alto_base){
	this->life = 250;
}

Worker::Worker() {}

Worker::~Worker() {}

void Worker::doInteract(){
	if (!activeInteractionEntity ||
		!this->canReach(activeInteractionEntity) ||
		this->isMoving() ||
		!activeInteractionEntity->estaViva()) {
			return;
	}
	this->state = STATE_INTERACTING;
	activeInteractionEntity->receiveInteraction(this);
}

void Worker::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

int Worker::getPoderCosecha() {
	return 1;
}

void Worker::receiveInteraction(Warrior* entity) {
	if (!this->estaViva()) return;
	this->life = this->life - this->vidaDescontada(entity);
	if((this->life % 100) == 0) {
		Log().Get(TAG) << "Worker receive interaction from Warrior vida: " << this->life;
	}
}
