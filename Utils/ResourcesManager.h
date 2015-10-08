/*
 * ResourcesManager.h
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#ifndef UTILS_RESOURCESMANAGER_H_
#define UTILS_RESOURCESMANAGER_H_

#include "../Models/Resource.h"
#include <SDL2/SDL.h>
#include <list>

class Escenario;
using namespace std;

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
