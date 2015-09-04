/*
 * Texture.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: kaoru
 */

#include "Texture.h"

Texture::Texture(std::string path,SDL_Renderer* sdlRenderer) {
	this->width = 0;
	this->height = 0;
	this->sdlTexture = NULL;

	if (!this->loadFromFile(path,sdlRenderer)){
		//falta la parte de cargar una imagen default
	}
}

Texture::~Texture() {
	this->free();
}

bool Texture::loadFromFile( std::string path,SDL_Renderer* sdlRenderer )
{
	//Get rid of preexisting texture
	this->free();

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
		return false;
	}

	this->width = loadedSurface->w;
	this->height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	this->sdlTexture = newTexture;
	return true;
}

void Texture::free() {
	if (this->sdlTexture == NULL){
		return;
	}

	SDL_DestroyTexture( this->sdlTexture );
	this->sdlTexture = NULL;
	this->width = 0;
	this->height = 0;
}

void Texture::render(SDL_Point origin,SDL_Renderer* sdlRenderer ) {
	SDL_Rect renderQuad = { origin.x, origin.y, this->width, this->height };
	SDL_RenderCopy( sdlRenderer, this->sdlTexture, NULL, &renderQuad );
}

int Texture::getWidth() {
	return this->width;
}

int Texture::getHeight() {
	return this->height;
}

