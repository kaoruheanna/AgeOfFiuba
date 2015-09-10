/*
 * Drawable.cpp
 *
 *  Created on: Sep 5, 2015
 *      Author: nico
 */

#include "Drawable.h"
#include <cstdio>
#include <SDL2/SDL_image.h>

Drawable::~Drawable() {
	// TODO Auto-generated destructor stub
}

Drawable::Drawable(int mainTilePositionX, int mainTilePositionY,int baseTileWidth, int baseTileHeight){
	this->baseTileHeight = baseTileHeight;
	this->baseTileWidth = baseTileWidth;
	this->mainTilePosition = {mainTilePositionX,mainTilePositionY};
	this->texture = NULL;
}

bool Drawable::loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer){
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return false;
	}

	//Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( sdlRenderer, loadedSurface );
	if( newTexture == NULL ){
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		SDL_FreeSurface( loadedSurface );
		return false;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );
	this->texture = newTexture;
	return true;
}

SDL_Rect Drawable::getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY){
	int w, h;
	SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
	return {
		windowMainTilePositionX - this->mainTilePosition.x,
		windowMainTilePositionY - this->mainTilePosition.y,
		w,h
	};
}

SDL_Texture* Drawable::getTexture(){
	return this->texture;
}
