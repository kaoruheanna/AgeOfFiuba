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

const int RESOURCES_QTY = 3;

class Escenario;
using namespace std;

class ResourcesManager {
public:
	ResourcesManager(Escenario *escenario);
	virtual ~ResourcesManager();

	list<Entity*> InsertResourcesForNewLoopOnMap();

	const char ** ResourceTypes() {
		static const char * EnumStrings[RESOURCES_QTY] = {"comida","madera","piedra"};
		return EnumStrings;
	}
private:
	int loopsToNext;

	Escenario *escenario;
	Resource* getNewResource();
	const char * getNewResourceType();
	const char * GetTextForEnum( int val );

};

#endif /* UTILS_RESOURCESMANAGER_H_ */
