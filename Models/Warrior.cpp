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
: MobileModel(id,nombre, posicion, ancho_base, alto_base){
	this->life = 1000;
}

Warrior::Warrior() {}

Warrior::~Warrior() {}

void Warrior::doInteract() {
	if (!this->estaViva()){
		return;
	}
	if (!this->activeInteractionEntity){
		return;
	}
	if (!this->canReach(this->activeInteractionEntity)){
		return;
	}
	if (this->isMoving()){
		return;
	}
	if (this->getTeam() == this->activeInteractionEntity->getTeam()){
		return;
	}
	if (!this->activeInteractionEntity->estaViva()){
		return;
	}

	//this->state = STATE_INTERACTING;
	this->activeInteractionEntity->receiveInteraction(this);
}

void Warrior::receiveInteraction(Warrior* entity) {
	entity->setInteracting();
	this->life = this->life - this->vidaDescontada(entity);
	if((this->life % 100) == 0) {
		Log().Get(TAG) << "Warrior receive interaction from Warrior vida: " << this->life;
	}
}

// Metodos de serializacion
int Warrior::getTotalBlockCount() {
	return MobileModel::getTotalBlockCount();
}

int Warrior::getBlockSizeFromIndex(int currentIndex) {
	return MobileModel::getBlockSizeFromIndex(currentIndex);
}

void Warrior::getBlockFromIndex(int currentIndex, void* buffer) {
	MobileModel::getBlockFromIndex(currentIndex, buffer);
}

void Warrior::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	MobileModel::deserialize(totalBlockCount,currentBlock,blockData);
}
