/*
 * Drawable.cpp
 *
 *  Created on: Sep 5, 2015
 *      Author: nico
 */

#include "Drawable.h"
#include <cstdio>

Drawable::~Drawable() {
	// TODO Auto-generated destructor stub
}

Drawable::Drawable(int mainTilePositionX, int mainTilePositionY,
		int baseTileWidth, int baseTileHeight, SDL_Texture* texture){
	this->baseTileHeight = baseTileHeight;
	this->baseTileWidth = baseTileWidth;
	this->mainTilePosition = {mainTilePositionX,mainTilePositionY};
	this->texture = texture;
}

SDL_Rect Drawable::getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY){
	int w, h;
	SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
	return {windowMainTilePositionX,windowMainTilePositionY,w,h};
}

SDL_Texture* Drawable::getTexture(){
	return this->texture;
}
