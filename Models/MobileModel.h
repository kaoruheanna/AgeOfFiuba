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
#include <map>
#include <list>

using namespace std;

class MobileModel: public Entity {
public:
	MobileModel();
	MobileModel(string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	virtual ~MobileModel();

	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	void setDestination(int destinationX, int destionationY);
	bool updatePosition();
	bool isMoving();
	void addResourceToCollect(string resourceName);
	void didCollectResource(string resourceName);
	list<string> getResourcesNames();
	int getValueForResource(string resourceName);
	void update(MobileModel* other);
	void addDestination(int destinationX, int destionationY);
	SDL_Point getNextDestination();
	void setPath(queue<SDL_Point> new_path);
	void clearPath();


private:
	queue <SDL_Point,deque<SDL_Point>> path;
	int destinationX;
	int destinationY;
	bool moving;
	std::map<string,int> resourcesCounter;

	int getSpeed();
};

#endif /* MOBILEMODEL_H_ */
