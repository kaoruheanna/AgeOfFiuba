/*
 * EntityFactory.h
 *
 *  Created on: Oct 8, 2015
 *      Author: dario
 */

#ifndef FACTORIES_ENTITYFACTORY_H_
#define FACTORIES_ENTITYFACTORY_H_

#include <list>
#include <string>
#include <iostream>
#include "../Models/Map.h"
#include <map>
#include <SDL2/SDL.h>
#include "../Models/Entity.h"
#include "../Models/Resource.h"
#include "../Models/MobileModel.h"

using namespace std;

class EntityFactory {
public:
	EntityFactory(Map *mundo, map<string, SDL_Point> sizeByType);
	virtual ~EntityFactory();
	MobileModel* crearProtagonista(const string& tipo, SDL_Point posicion);
	Entity* crearEntidad(const string& tipo, SDL_Point posicion, bool esProtagonista = false);
	Resource* crearRecurso(const string& tipo, SDL_Point posicion);
private:
	map<string, SDL_Point> sizeByType;
	Map *mundo;

	SDL_Point getPosition(const string& tipo, SDL_Point posicion);
	SDL_Point getSize(const string& tipo, SDL_Point posicion);
};

#endif /* FACTORIES_ENTITYFACTORY_H_ */
