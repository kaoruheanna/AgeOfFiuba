/*
 * ResourcesManager.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#include "ResourcesManager.h"
#include "../Utils/Log.h"

const int LOOPS_DIFF = 200;
const string TAG = "ResourcesManager";

ResourcesManager::ResourcesManager(int ancho,int alto) {
	this->loopsToNext = 0;
	this->ancho = ancho;
	this->alto = alto;
}

ResourcesManager::~ResourcesManager() {}

Resource* ResourcesManager::getNewResource() {
	int posicionX = rand() % this->ancho;
	int posicionY = rand() % this->alto;
	Log().Get(TAG) << "Agregar en posicion" << posicionX << "," << posicionY;

	return new Resource("arbol", { posicionX, posicionY });
}

list<Entity*> ResourcesManager::InsertResourcesForNewLoopOnMap() {
	list<Entity*> entities;

	if(loopsToNext >= LOOPS_DIFF) {
		entities.push_back(getNewResource());
		loopsToNext = 0;
	}
	loopsToNext++;
	return entities;
}

