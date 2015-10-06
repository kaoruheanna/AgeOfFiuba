/*
 * Resource.h
 *
 *  Created on: Oct 5, 2015
 *      Author: dario
 */

#ifndef MODELS_RESOURCE_H_
#define MODELS_RESOURCE_H_
#include "Entity.h"

using namespace std;

class Resource : public Entity {
public:
	Resource();
	Resource(string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	virtual ~Resource();
};

#endif /* MODELS_RESOURCE_H_ */
