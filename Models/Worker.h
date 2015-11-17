/*
 * Worker.h
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#ifndef MODELS_WORKER_H_
#define MODELS_WORKER_H_

#include "MobileModel.h"
#include "Warrior.h"

class Worker: public MobileModel {
public:
	Worker(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Worker();
	virtual ~Worker();

	virtual void interact(Entity* entity);
	virtual void receiveInteraction(Warrior* entity);
	virtual void doInteract();

	virtual int getPoderCosecha();
};

#endif /* MODELS_WORKER_H_ */
