/*
 * FogOfWar.h
 *
 *  Created on: 07/10/2015
 *      Author: die89
 */

#ifndef FOGOFWAR_H_
#define FOGOFWAR_H_

#include <list>
#include "Entity.h"
#include "Map.h"

class FogOfWar {
public:
	FogOfWar(list<Entity*> entities, Entity* mainCharacter, Map* map);
	void initialice();
	void update();
	~FogOfWar();


private:
	list<Entity*> entities;
	Entity* character;
	Map* world;

	bool checkInSight(SDL_Point character, SDL_Point entity);
};

#endif /* FOGOFWAR_H_ */
