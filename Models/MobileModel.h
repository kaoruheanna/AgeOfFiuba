/*
 * MobileModel.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef MOBILEMODEL_H_
#define MOBILEMODEL_H_

#include <string>
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
	void updatePosition();
	bool isMoving();
	void addResourceToCollect(const char* resourceName);
	void didCollectResource(const char* resourceName);
	list<const char *> getResourcesNames();
	int getValueForResource(const char* resourceName);

private:
	int destinationX;
	int destinationY;
	bool moving;
	std::map<const char*,int> resourcesCounter;

	int getSpeed();
};

#endif /* MOBILEMODEL_H_ */
