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
	ResourcesManager(Escenario *escenario);
	virtual ~ResourcesManager();

	list<Entity*> InsertResourcesForNewLoopOnMap();
private:
	int loopsToNext;

	Escenario *escenario;
	Resource* getNewResource();
};

#endif /* UTILS_RESOURCESMANAGER_H_ */
