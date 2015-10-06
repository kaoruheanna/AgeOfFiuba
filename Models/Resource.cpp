/*
 * Resource.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#include "Resource.h"

Resource::Resource(string nombre, SDL_Point posicion, int ancho_base, int alto_base): Entity(nombre, posicion, ancho_base, alto_base) {}

Resource::~Resource() {
	// TODO Auto-generated destructor stub
}
