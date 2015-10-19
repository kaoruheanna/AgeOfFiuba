/*
 * MobileModel.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef MOBILEMODEL_H_
#define MOBILEMODEL_H_

#include <string>
#include <queue>
#include "Entity.h"
#include "../Red/Serializable.h"
#include <map>
#include <list>

using namespace std;

class MobileModel: public Entity, public Serializable {
public:
	MobileModel();
	MobileModel(string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	virtual ~MobileModel();

	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	void setDestination(int destinationX, int destionationY);
	int getDestinationX();
	int getDestinationY();
	bool updatePosition();
	bool isMoving();
	void addResourceToCollect(string resourceName);
	void didCollectResource(string resourceName);
	list<string> getResourcesNames();
	int getValueForResource(string resourceName);
	void update(MobileModel* other);
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	queue <SDL_Point,deque<SDL_Point>> camino;
	int destinationX;
	int destinationY;
	bool moving;
	std::map<string,int> resourcesCounter;

	int getSpeed();

	// Serializable methods
	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);
};

#endif /* MOBILEMODEL_H_ */
