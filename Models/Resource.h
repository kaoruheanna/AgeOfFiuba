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
	Resource( const Resource& other );
	Resource(int id, string nombre, SDL_Point posicion, int ancho_base = 1, int alto_base = 1);
	virtual ~Resource();
	virtual EntityType getClass();

	virtual void interact(Entity* entity);
	virtual void receiveInteraction(Warrior* entity);
	void cosechar();

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
};

#endif /* MODELS_RESOURCE_H_ */
