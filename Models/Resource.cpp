/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"

Resource::Resource() : Entity("", {0, 0}, 1, 1) {
	this->id = 0;
}
Resource::Resource(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(nombre, posicion, ancho_base, alto_base) {
	this->id = id;
}

Resource::Resource( const Resource& other ) : Entity(other) {
	this->id = other.id;
};

Resource::~Resource() {}

int Resource::getTotalBlockCount() {
	return Entity::getTotalBlockCount() + 1;
}

int Resource::getBlockSizeFromIndex(int currentIndex) {
	int realIndex =  currentIndex - Entity::getTotalBlockCount();
	if (realIndex == 0) {
		return sizeof(int);
	}
	return Entity::getBlockSizeFromIndex(currentIndex);
}

void Resource::getBlockFromIndex(int currentIndex, void* buffer) {
	int realIndex =  currentIndex - Entity::getTotalBlockCount();
	if (realIndex == 0) {
		memcpy(buffer, &this->id, sizeof(int));
	} else {
		Entity::getBlockFromIndex(currentIndex, buffer);
	}
}

void Resource::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	int realIndex =  currentBlock - Entity::getTotalBlockCount();
	if (realIndex == 0) {
		memcpy(&this->id, blockData, sizeof(int));
	} else {
		Entity::deserialize(totalBlockCount,currentBlock,blockData);
	}
}

EntityType Resource::getClass() {
	return RESOURCE;
}

