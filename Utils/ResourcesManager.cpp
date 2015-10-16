/*
 * ResourcesManager.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#include "../Models/Escenario.h"
#include "../Utils/Log.h"

const int LOOPS_DIFF = 200;
const string TAG = "ResourcesManager";

ResourcesManager::ResourcesManager(Escenario *escenario) {
	this->loopsToNext = 0;
	this->escenario = escenario;
}

ResourcesManager::~ResourcesManager() {}

const char * ResourcesManager::GetTextForEnum( int val ) {
	return this->ResourceTypes()[val];
}

const char * ResourcesManager::getNewResourceType() {
	return this->GetTextForEnum(rand() % RESOURCES_QTY);
}

Resource* ResourcesManager::getNewResource() {
	int posicionX = rand() % this->escenario->mundo->getWidth();
	int posicionY = rand() % this->escenario->mundo->getHeight();
	Resource *resource = this->escenario->factory->crearRecurso(this->getNewResourceType(),{ posicionX, posicionY });
	if(resource == NULL){
		Log().Get("Escenario", logWARNING) << "La entidad  del escenario  no pudo ser creada.";
	} else {
		if(!this->escenario->construirEntidad(resource, resource->getPosicion())){
			delete resource;
			Log().Get("Escenario", logWARNING) << "La entidad N°  del escenario  no fue agregada al mapa. La misma no puede estar en la misma posicion que otra entidad.";
		}
	}
	Log().Get("Escenario") << "id: " << resource->id << " Se creo en " << posicionX << " , " << posicionY;

	return resource;
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

