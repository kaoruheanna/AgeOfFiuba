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
	this->missingImageDrawable = NULL;

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
	this->missingImageDrawable = new Drawable(64,0,1,1, this->loadTextureFromFile("img/missingImage.png"));
	SDL_Texture* loaded = this->loadTextureFromFile("img/Mario-Mapache.png");
	if(loaded != NULL){
		this->drawablesByInstanceName.insert(
			std::pair<std::string,Drawable*>(
				"mario",
				new Drawable(0,0,1,1,loaded)
			)
		);
	}
	return success;
}

SDL_Texture* Renderer::loadTextureFromFile( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
	}

	//Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( sdlRenderer, loadedSurface );
	if( newTexture == NULL ){
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return newTexture;
}

void Renderer::close() {
	//Free loaded images
	for(map<std::string, Drawable*>::iterator it = this->drawablesByInstanceName.begin(); it != this->drawablesByInstanceName.end(); ++it) {
	  SDL_DestroyTexture( it->second->getTexture() );
	  delete it->second;
	}

	SDL_DestroyTexture( this->missingImageDrawable->getTexture() );
	delete this->missingImageDrawable;


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
	std::map<std::string,Drawable *>::iterator found = this->drawablesByInstanceName.find(view->getType());
	Drawable* drawable = NULL;
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	} else {
		printf("Cannot find image for view %s will use the default\n", view->getType().c_str());
		drawable = this->missingImageDrawable;
	}
	view->setDrawable(drawable);
	this->views.push_back(view);
}
