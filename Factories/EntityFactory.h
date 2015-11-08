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
#include "../Models/Warrior.h"
#include "../Models/Worker.h"
#include "../Configuration/TipoConfig.h"

using namespace std;

class EntityFactory {
public:
	EntityFactory(Map *mundo, list<TipoConfig> tiposConfigList);
	virtual ~EntityFactory();
	MobileModel* crearProtagonista(const string& tipo, SDL_Point posicion);
	Entity* crearEntidad(const string& tipo, SDL_Point posicion);
	Entity* crearEntidad(const string& tipo, SDL_Point posicion, const string& equipo);
	Resource* crearRecurso(const string& tipo, SDL_Point posicion);
private:
	Map *mundo;
	map<string, TipoConfig> tipos;

	SDL_Point getPositionForTile(const string& tipo, SDL_Point posicion, bool centered = false);
	SDL_Point getSize(const string& tipo);
};

#endif /* FACTORIES_ENTITYFACTORY_H_ */
