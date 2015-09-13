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
	this->height = 0;
	this->width = 0;
}

bool Drawable::loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer){
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return false;
	}

	bool result = this->loadTextureFromSurface(sdlRenderer, loadedSurface);
	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );
	return result;
}

SDL_Rect* Drawable::getClipRect(){
	return NULL;
}

bool Drawable::loadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
	// Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface( renderer, surface );
	if( newTexture == NULL ){
		printf( "Unable to create texture for drawable! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	this->texture = newTexture;
	this->onTextureChange();
	return true;
}

void Drawable::onTextureChange(){
	int w, h;
	SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
	this->height = h;
	this->width = w;
}

SDL_Rect Drawable::getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY){
	return {
		windowMainTilePositionX - this->mainTilePosition.x,
		windowMainTilePositionY - this->mainTilePosition.y,
		this->width,
		this->height
	};
}

SDL_Texture* Drawable::getTexture(){
	return this->texture;
}

void Drawable::free(){
	SDL_DestroyTexture( this-> texture );
	this->texture = NULL;
}

// dummy functions
void Drawable::animate(){
}

void Drawable::selectAnimation(MotionDirection direction) {
}

