/*
 * Renderer.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#include "Renderer.h"
#include "Sprite.h"
#include "../Utils/Log.h"

const std::string TAG = "Renderer";

Renderer::Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos) {
	this->window = NULL;
	this->sdlRenderer = NULL;
	this->missingImageDrawable = NULL;
	this->mainTilePosition = {screenWidth/2,0}; // para que el mapa este en la mitad
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	bool didInitSDL = this->initSDL();
	bool didLoadMedia = this->loadMedia(tipos);
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
	this->window = SDL_CreateWindow( "Age Of FIUBA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_SHOWN );
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

bool Renderer::loadMedia(list<TipoConfig> tipos) {
	bool success = true;

	// Imagen Default
	Log().Get(TAG,logINFO) << "Cargando drawable default";
	this->missingImageDrawable = new Drawable(64,0,1,1);
	success = this->missingImageDrawable->loadTextureFromFile("img/missingImage.png",this->sdlRenderer);
	if(!success){
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	} else {
		Log().Get(TAG,logINFO) << "Cargado drawable default";
	}

	// Mapa
	Log().Get(TAG,logINFO) << "Cargando tile default";
	string tileDefault = "img/grass1.png";
	Drawable *tileDefDrawable = new Drawable(64,0,1,1);//nose si esta bien
	if (tileDefDrawable -> loadTextureFromFile(tileDefault,this->sdlRenderer)){
		Log().Get(TAG,logINFO) << "Cargado tile default";
		this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>("tileDefault", tileDefDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	}

	//imagen casa
	Log().Get(TAG,logINFO) << "Cargando dibujo de casa";
	//string casa = "img/casa2x2.png";
	string casa = "img/testIm.png";
	Drawable *casaDrawable = new Drawable(153,31,2,2);//nose si esta bien
	if (casaDrawable -> loadTextureFromFile(casa,this->sdlRenderer)){
		Log().Get(TAG,logINFO) << "Cargado tile default";
		this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>("casa", casaDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	}


	// soldado => Esta en el YAML (Lo dejo por si le craseha a alguno)
	/*string soldierPath = "img/ManSprite.png";
	Drawable *soldierDrawable = new Sprite(25,50,1,1,50,50,10);
	if (soldierDrawable->loadTextureFromFile(soldierPath,this->sdlRenderer)){
		this->drawablesByInstanceName.insert(
			std::pair<std::string,Drawable*>("soldier",	soldierDrawable)
		);
	}*/

	// Cargar los tipos pasados por el YAML
	int i = 0;
	for (list<TipoConfig>::iterator it = tipos.begin(); it != tipos.end(); ++it) {
	  Log().Get(TAG,logDEBUG) << "Parseando tipo: " << i;
	  TipoConfig newNodo = *it;
	  if(newNodo.getNombre() == "" || newNodo.getImagen() == ""){
		  Log().Get(TAG,logERROR) << "Tipo N°" << i << " es incorrecto. Deberia tener nombre e imagen.";
	  } else {
		  Drawable *nodoDrawable = NULL;
		  if(newNodo.getFPS() > 0){
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " es un sprite.";
			  nodoDrawable = new Sprite(
				newNodo.getPixelRefX(), newNodo.getPixelRefY(),
				newNodo.getAnchoBase(), newNodo.getAltoBase(),
				newNodo.getAnchoFrame(), newNodo.getAltoFrame(),
				newNodo.getFPS()
			  );
			  // TODO agregar el campo delay a Sprite
		  } else {
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " es un drawable.";
			  nodoDrawable = new Drawable(
				newNodo.getPixelRefX(), newNodo.getPixelRefY(),
				newNodo.getAnchoBase(), newNodo.getAltoBase()
			  );
		  }
		  if(nodoDrawable->loadTextureFromFile(newNodo.getImagen(), this->sdlRenderer)){
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " se cargo correctamente bajo el nombre " << newNodo.getNombre();
			  this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>(newNodo.getNombre(), nodoDrawable)
			  );
		  } else {
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " no se pudo cargar la imagen.";
		  }
	  }
	  i++;
	}
	return success;
}

void Renderer::close() {
	//Free loaded images
	for(map<std::string, Drawable*>::iterator it = this->drawablesByInstanceName.begin(); it != this->drawablesByInstanceName.end(); ++it) {
	  it->second->free();
	  delete it->second;
	}

	this->missingImageDrawable->free();
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

SDL_Point Renderer::mapToWindowPoint(SDL_Point mapPoint){
	SDL_Point windowPoint = {0,0};
	// Rotar punto 45 grados
	windowPoint.x = mapPoint.x * ISOMETRIC_ROTATION_FACTOR - mapPoint.y * ISOMETRIC_ROTATION_FACTOR;
	windowPoint.y = mapPoint.x * ISOMETRIC_ROTATION_FACTOR + mapPoint.y * ISOMETRIC_ROTATION_FACTOR;
	// Ajustar a la posicion del 0,0 en la pantalla
	windowPoint.x += this->mainTilePosition.x;
	windowPoint.y += this->mainTilePosition.y;
	return windowPoint;
}
SDL_Point Renderer::windowToMapPoint(SDL_Point windowPoint){
	SDL_Point centeredWindow = {windowPoint.x, windowPoint.y};
	// Ajustar la pantalla a la posicion del 0,0
	centeredWindow.x -= this->mainTilePosition.x;
	centeredWindow.y -= this->mainTilePosition.y;
	SDL_Point mapPoint = {0,0};
	// Rotar punto -45 grados
	mapPoint.x = centeredWindow.x * ISOMETRIC_ROTATION_FACTOR + centeredWindow.y * ISOMETRIC_ROTATION_FACTOR;
	mapPoint.y = -centeredWindow.x * ISOMETRIC_ROTATION_FACTOR + centeredWindow.y * ISOMETRIC_ROTATION_FACTOR;
	return mapPoint;
}

void Renderer::draw(int mapPositionX, int mapPositionY, Drawable* drawable, bool iso) {
	SDL_Point windowPoint = {mapPositionX, mapPositionY};
	if(iso){
		windowPoint = this->mapToWindowPoint({mapPositionX, mapPositionY});
	} else {
		windowPoint = {mapPositionX+this->mainTilePosition.x, mapPositionY + this->mainTilePosition.y};
	}
	SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);
	//printf("mapPoint: %i:%i windowPoint: %i:%i\n", mapPositionX, mapPositionY, windowPoint.x, windowPoint.y);
	//SDL_Point mapPoint = this->windowToMapPoint(windowPoint);
	//printf("windowPoint: %i:%i mapPoint: %i:%i\n", windowPoint.x, windowPoint.y, mapPoint.x, mapPoint.y);
	if(this->isInsideWindow(&renderQuad)){
		//Log().Get(TAG,logDEBUG) << "Drawable inside window with rect { " << renderQuad.x << ", " << renderQuad.y << ", " << renderQuad.w << ", " << renderQuad.h << " }";
		SDL_RenderCopy(sdlRenderer, drawable->getTexture(), drawable->getClipRect(), &renderQuad);
	} else {
		//Log().Get(TAG,logDEBUG) << "Drawable outside window with rect { " << renderQuad.x << ", " << renderQuad.y << ", " << renderQuad.w << ", " << renderQuad.h << " }";
	}
}

bool Renderer::isInsideWindow(SDL_Rect* rect){
	return (rect->x < this->screenWidth &&
			rect->x + rect->w > 0 &&
		    rect->y < this->screenHeight &&
			rect->y + rect->h > 0);
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
		Log().Get(TAG,logWARNING) << "No se pudo cargar la imagen: '"<<view->getType().c_str()<<"', usa la imagen default";
		drawable = this->missingImageDrawable;
	}
	view->setDrawable(drawable);
	this->views.push_back(view);
}
