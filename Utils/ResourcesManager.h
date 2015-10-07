/*
 * ResourcesManager.h
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#ifndef UTILS_RESOURCESMANAGER_H_
#define UTILS_RESOURCESMANAGER_H_

#include "../Models/Escenario.h"
#include "../Models/Resource.h"
#include <SDL2/SDL.h>

class ResourcesManager {
public:
	ResourcesManager(int ancho,int alto);
	virtual ~ResourcesManager();

	list<Entity*> InsertResourcesForNewLoopOnMap();
private:
	int loopsToNext;
	int alto;
	int ancho;

	Resource* getNewResource();
};

#endif /* UTILS_RESOURCESMANAGER_H_ */
