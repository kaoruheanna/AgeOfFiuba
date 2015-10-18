/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"

Resource::Resource(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(nombre, posicion, ancho_base, alto_base) {
	this->id = id;
	this->Cosechable = true;
}

Resource::Resource( const Resource& other ) : Entity(other) {
	this->id = other.id;
};

Resource::~Resource() {}

