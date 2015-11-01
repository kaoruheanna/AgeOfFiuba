/*
 * Warrior.h
 *
 *  Created on: Oct 25, 2015
 *      Author: dario
 */

#ifndef MODELS_WARRIOR_H_
#define MODELS_WARRIOR_H_

#include "MobileModel.h"
#include "Building.h"

class Warrior: public MobileModel {
public:
	Warrior(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Warrior();

	virtual void interact(Entity* entity);
	virtual void receiveInteraction(Building* entity);

	virtual ~Warrior();

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
};

#endif /* MODELS_WARRIOR_H_ */
