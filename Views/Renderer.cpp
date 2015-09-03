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
	this->window = NULL;
	this->sdlRenderer = NULL;
	this->marioTexture = NULL;

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
	this->window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if (this->window == NULL) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	this->sdlRenderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
	if( this->sdlRenderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor( this->sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags))	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	//Get window surface
//	this->gScreenSurface = SDL_GetWindowSurface( window );
	return true;
}

bool Renderer::loadMedia() {
	bool success = true;
	this->marioTexture = new Texture();
	if (!this->marioTexture->loadFromFile("img/Mario-Mapache.png",this->sdlRenderer)){
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}

	return success;
}

void Renderer::close() {
	//Free loaded images
	this->marioTexture->free();

	//Destroy window
	SDL_DestroyRenderer(this->sdlRenderer);
	SDL_DestroyWindow(this->window);
	this->window = NULL;
	this->sdlRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Renderer::draw() {
	//Clear screen
	SDL_SetRenderDrawColor(this->sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(this->sdlRenderer);

	this->marioTexture->render( 100, 400, this->sdlRenderer);

	//Update screen
	SDL_RenderPresent(this->sdlRenderer);
}

bool Renderer::canDraw() {
	return this->successfullInit;
}
