/*
 * ResourcesManager.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#include "ResourcesManager.h"
#include "../Models/Resource.h"
#include "../Utils/Log.h"

const int LOOPS_DIFF = 200;
const string TAG = "ResourcesManager";

ResourcesManager::ResourcesManager(int ancho,int alto) {
	this->loopsToNext = 0;
	this->ancho = ancho;
	this->alto = alto;
}

ResourcesManager::~ResourcesManager() {}

list<Entity*> ResourcesManager::InsertResourcesForNewLoopOnMap() {
	list<Entity*> entities;

	if(loopsToNext >= LOOPS_DIFF) {
		int posicionX = rand() % this->ancho;
		int posicionY = rand() % this->alto;

		entities.push_back(new Entity("arbol",{posicionX,posicionY},1,1));
		Log().Get(TAG) << "Agregar en posicion" << posicionX << "," << posicionY;
		loopsToNext = 0;
	}
	loopsToNext++;
	return entities;
}

