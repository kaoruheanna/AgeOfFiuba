/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"

Resource::Resource() : Entity("", {0, 0}, 1, 1) {
	this->id = 0;
	this->Cosechable = true;
}
Resource::Resource(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(nombre, posicion, ancho_base, alto_base) {
	this->id = id;
	this->Cosechable = true;
}

Resource::Resource( const Resource& other ) : Entity(other) {
	this->id = other.id;
};

void Resource::cosechar() {
	this->Cosechable = false;
}

Resource::~Resource() {}

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

