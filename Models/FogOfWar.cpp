/* FogOfWar.cpp
 *
 *  Created on: 07/10/2015
 *      Author: die89
 */

#include "FogOfWar.h"

FogOfWar::FogOfWar(list<Entity*> entities, Entity* mainCharacter, Map* map){
	this->entities = entities;
	this->character = mainCharacter;
	this->world = map;
}

void FogOfWar::initialice(){
	list<Entity*>::iterator currentEntityIterator;
	Entity* currentEntity;
	for (currentEntityIterator = entities.begin(); currentEntityIterator != entities.end(); ++currentEntityIterator){
			currentEntity = (*currentEntityIterator);
			currentEntity->setEstado(OCULTO);
	}
	character->setEstado(VISIBLE);
}

void FogOfWar::update(){
	list<Entity*>::iterator currentEntityIterator;
		Entity* currentEntity;
		SDL_Point currentCharacterPositionInTile = this->world->getTileForPosition(this->character->getPosicion());
		SDL_Point currentEntityPositionInTile = {0,0};
		for (currentEntityIterator = entities.begin(); currentEntityIterator != entities.end(); ++currentEntityIterator){
			currentEntity = (*currentEntityIterator);
			currentEntityPositionInTile = this->world->getTileForPosition(currentEntity->getPosicion());
			if (this->checkInSight( currentCharacterPositionInTile , currentEntityPositionInTile )){
				currentEntity->setEstado(VISIBLE);
			}
			else {
				if (currentEntity->getEstado() == VISIBLE ){
					currentEntity->setEstado(NUBLADO);
				}
			}
		}

}

bool FogOfWar::checkInSight(SDL_Point character, SDL_Point entity){

	return true;
}

FogOfWar::~FogOfWar() {
	// TODO Auto-generated destructor stub
}

