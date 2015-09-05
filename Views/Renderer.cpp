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
	this->marioDrawable = NULL;

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

	return true;
}

bool Renderer::loadMedia() {
	bool success = true;
	this->loadTextureFromFile("img/Mario-Mapache.png", "mario");
	this->marioDrawable = new Drawable(0,0,1,1,this->marioTexture);
	return success;
}

bool Renderer::loadTextureFromFile( std::string path, std::string name )
{
	// TODO add texture to a map
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

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	this->marioTexture = newTexture;
	return true;
}

void Renderer::close() {
	//Free loaded images
	SDL_DestroyTexture( this->marioTexture );
	this->marioTexture = NULL;
	this->marioDrawable = NULL;

	//Destroy window
	SDL_DestroyRenderer(this->sdlRenderer);
	SDL_DestroyWindow(this->window);
	this->window = NULL;
	this->sdlRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Renderer::drawViews() {
	//Clear screen
	SDL_SetRenderDrawColor(this->sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(this->sdlRenderer);

	list<View*>::iterator i;
	for(i=this->views.begin(); i != this->views.end(); ++i) {
		View* view = *i;
		view->render(this);
	}

	//Update screen
	SDL_RenderPresent(this->sdlRenderer);
}

void Renderer::draw(int mapPositionX, int mapPositionY, Drawable* drawable) {
	// TODO translate mapPositionX and mapPositionY to the window cordinates
	SDL_Rect renderQuad = drawable->getRectToDraw(mapPositionX, mapPositionY);
	SDL_RenderCopy(sdlRenderer, drawable->getTexture(), NULL, &renderQuad);
}

bool Renderer::canDraw() {
	return this->successfullInit;
}

void Renderer::addView(View* view) {
	view->setDrawable(this->marioDrawable);
	this->views.push_back(view);
}
