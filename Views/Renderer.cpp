/*
 * Renderer.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#include "Renderer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Renderer::Renderer() {
	this->gWindow = NULL;
	this->gScreenSurface = NULL;
	this->gPNGSurface = NULL;
	bool didInitSDL = this->initSDL();
	bool didLoadMedia = this->loadMedia();
	this->successfullInit = (didInitSDL && didLoadMedia);
}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

bool Renderer::initSDL() {
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create window
	this->gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if (this->gWindow == NULL) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags))	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	//Get window surface
	this->gScreenSurface = SDL_GetWindowSurface( gWindow );
	return true;
}

bool Renderer::loadMedia() {
	//Load PNG surface
	this->gPNGSurface = loadSurface( "img/Mario-Mapache.png" );
	if (this->gPNGSurface == NULL) {
		printf( "Failed to load PNG image!\n" );
		return false;
	}
	return true;
}

SDL_Surface* Renderer::loadSurface(string path) {
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	//Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
	if (optimizedSurface == NULL){
		printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return optimizedSurface;
}

void Renderer::close() {
	//Free loaded image
	SDL_FreeSurface(this->gPNGSurface);
	this->gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(this->gWindow);
	this->gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Renderer::draw() {
	//Apply the PNG image
	SDL_BlitSurface( this->gPNGSurface, NULL, this->gScreenSurface, NULL );

	//Update the surface
	SDL_UpdateWindowSurface( this->gWindow );
}

bool Renderer::canDraw() {
	return this->successfullInit;
}
