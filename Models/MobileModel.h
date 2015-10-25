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

class MobileModel: public Entity{
public:
	MobileModel();
	MobileModel(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
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
	void addDestination(int destinationX, int destionationY);
	SDL_Point getNextDestination();
	SDL_Point getNextPosition();
	void setPath(queue<SDL_Point> new_path);
	void clearPath();
	bool esJugador();
	string getNombreAMostrar();
	string getUsername();
	void setUsername(string username);

	bool isActive();
	void setActive(bool active);
	void olvidarCamino();
	virtual EntityType getClass();

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);

private:
	queue <SDL_Point,deque<SDL_Point>> path;
	int destinationX;
	int destinationY;
	bool moving;
	string username;
	bool userActive;
	std::map<string,int> resourcesCounter;

	int getSpeed();

};

#endif /* MOBILEMODEL_H_ */
