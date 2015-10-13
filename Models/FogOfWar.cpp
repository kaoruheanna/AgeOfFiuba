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
	int posicionx;
	int posiciony;
	for (posicionx = 0; posicionx < this->world->getWidth(); ++posicionx){
		for (posiciony = 0; posiciony < this->world->getHeight(); ++posiciony){
			this->world->setEstado(posiciony,posicionx,OCULTO);
		}
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
		int posicionx;
		int posiciony;
		for (posicionx = 0; posicionx < this->world->getWidth(); ++posicionx){
			for (posiciony = 0; posiciony < this->world->getHeight(); ++posiciony){
				if (this->world->getEstado(posiciony,posicionx) == VISIBLE){
					this->world->setEstado(posiciony,posicionx,NUBLADO);
				}
			}
		}
		for (posicionx = (currentCharacterPositionInTile.x - 1); posicionx <= (currentCharacterPositionInTile.x + 1); ++posicionx){
			for (posiciony = (currentCharacterPositionInTile.y - 1); posiciony <= (currentCharacterPositionInTile.y + 1); ++posiciony){
				this->world->setEstado(posiciony,posicionx,VISIBLE);
			}
		}

}

bool FogOfWar::checkInSight(SDL_Point character, SDL_Point entity){
	if (((entity.x >= (character.x - 1)) && (entity.x <= (character.x + 1))) && ((entity.y >= (character.y - 1)) && (entity.y <= (character.y + 1)))){
		return true;
	}
	return false;
}

FogOfWar::~FogOfWar() {
	// TODO Auto-generated destructor stub
}

