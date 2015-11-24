/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

const string TAG = "Resource";

Resource::Resource() : Entity(0,"", {0, 0}, 1, 1) {
	this->hardness = RESOURCE_HARDNESS;
	this->resourceQuantity = this->hardness;
}
Resource::Resource(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(id,nombre, posicion, ancho_base, alto_base) {
	this->hardness = RESOURCE_HARDNESS;
	this->resourceQuantity = this->hardness;
}

Resource::Resource( const Resource& other ) : Entity(other) {
	this->id = other.id;
	this->hardness = RESOURCE_HARDNESS;
	this->resourceQuantity = this->hardness;
};

Resource::~Resource() {}



void Resource::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

void Resource::receiveInteraction(Warrior* entity) {
	Log().Get(TAG) << "Resource receive interaction from Warrior -> Do nothing";
}

void Resource::receiveInteraction(Worker *entity) {
	if (this->estaViva()){
		if (this->resourceQuantity <= 0){
			this->life -= entity->getPoderCosecha();
			this->resourceQuantity = this->hardness;
			if (this->getNombre().compare(RECURSO_COMIDA) == 0){
				entity->foodGathered = entity->getPoderCosecha();
			}
			else if (this->getNombre().compare(RECURSO_ARBOL) == 0){
				entity->woodGathered = entity->getPoderCosecha();
			}
			else if (this->getNombre().compare(RECURSO_PIEDRA) == 0){
				entity->stoneGathered = entity->getPoderCosecha();
			}
			else if (this->getNombre().compare(RECURSO_ORO) == 0){
				entity->goldGathered = entity->getPoderCosecha();
			}
		}
		else{
			this->resourceQuantity -= entity->getPoderCosecha();
		}
		entity->setInteracting();
	}
}

// Serializar

int Resource::getTotalBlockCount() {
	return Entity::getTotalBlockCount();
}

int Resource::getBlockSizeFromIndex(int currentIndex) {
	return Entity::getBlockSizeFromIndex(currentIndex);
}

void Resource::getBlockFromIndex(int currentIndex, void* buffer) {
	Entity::getBlockFromIndex(currentIndex, buffer);
}

void Resource::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	Entity::deserialize(totalBlockCount,currentBlock,blockData);
}

EntityType Resource::getClass() {
	return RESOURCE;
}

