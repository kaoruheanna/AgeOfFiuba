/*
 * Drawable.cpp
 *
 *  Created on: Sep 5, 2015
 *      Author: nico
 */

#include "Drawable.h"
#include <cstdio>
#include <SDL2/SDL_image.h>
#include "../Utils/Log.h"

const std::string TAG = "Drawable";

Drawable::~Drawable() {
	// TODO Auto-generated destructor stub
	this->mainTilePosition = {0,0};
	this->height = 0;
	this->width = 0;
	this->free();
}

Drawable::Drawable(int mainTilePositionX, int mainTilePositionY){
	this->mainTilePosition = {mainTilePositionX,mainTilePositionY};
	this->texture = NULL;
	this->height = 0;
	this->width = 0;
}

bool Drawable::loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer){
	//TODO aca estaba trabajando
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		Log().Get(TAG,logWARNING) << "No se pudo cargar la imagen:'"<<path.c_str()<<"' - error: "<<IMG_GetError();
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
	if(this->texture != NULL){
		SDL_DestroyTexture( this-> texture );
		this->texture = NULL;
	}
}

void Drawable::animate(AnimationStatus status){
}

AnimationStatus Drawable::getAnimation(MotionDirection currentDirection, bool currentlyMoving, AnimationStatus lastStatus){
	AnimationStatus nextStatus;
	nextStatus.direction = SOUTH;
	nextStatus.animationIndex = 0;
	nextStatus.isMoving = false;
	return nextStatus;
}

int Drawable::getHeight() {
	return this->height;
}
