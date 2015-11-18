/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"
#include "../Utils/Log.h"

const string TAG = "Resource";

Resource::Resource() : Entity(0,"", {0, 0}, 1, 1) {}
Resource::Resource(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(id,nombre, posicion, ancho_base, alto_base) {}

Resource::Resource( const Resource& other ) : Entity(other) {
	this->id = other.id;
};

Resource::~Resource() {}



void Resource::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

void Resource::receiveInteraction(Warrior* entity) {
	Log().Get(TAG) << "Resource receive interaction from Warrior -> Do nothing";
}

void Resource::receiveInteraction(Worker *entity) {
	int antigua = this->life;
	int actual = this->life - entity->getPoderCosecha();
	this->life = actual;
	if (antigua > 0){
		if (this->getNombre().compare("comida") == 0){
			entity->foodGathered = antigua-actual;

		}
		else if (this->getNombre().compare("arbol") == 0){
				entity->woodGathered = antigua-actual;
		}
		else if (this->getNombre().compare("piedra") == 0){
				entity->stoneGathered = antigua-actual;
		}
		else if (this->getNombre().compare("oro") == 0){
				entity->goldGathered = antigua-actual;
		}
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

