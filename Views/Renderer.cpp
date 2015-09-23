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

const int TILE_WIDTH = 128;//hay que ver de donde sacamos esto
const int TILE_HEIGHT = 64;//deberia ser info que tiene el renderer



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
		Log().Get(TAG,logERROR) << "SDL no pudo inicializar: "<<SDL_GetError();
		return false;
	}

	//Create window
	this->window = SDL_CreateWindow( "Age Of FIUBA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->screenWidth, this->screenHeight, SDL_WINDOW_SHOWN );
	if (this->window == NULL) {
		Log().Get(TAG,logERROR) << "No se pudo crear la ventana: "<<SDL_GetError();
		return false;
	}

	this->sdlRenderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
	if( this->sdlRenderer == NULL ) {
		Log().Get(TAG,logERROR) << "No se pudo crear el SDL renderer: "<<SDL_GetError();
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor( this->sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags))	{
		Log().Get(TAG,logERROR) << "SDL_image no pudo inicializar: "<<SDL_GetError();
		return false;
	}

	return true;
}

bool Renderer::loadMedia(list<TipoConfig> tipos) {
	bool success = true;

	// Imagen Default
	Log().Get(TAG,logINFO) << "Cargando drawable default";
	this->missingImageDrawable = new Drawable(64,0);
	success = this->missingImageDrawable->loadTextureFromFile("img/missingImage.png",this->sdlRenderer);
	if(!success){
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	} else {
		Log().Get(TAG,logINFO) << "Cargado drawable default";
	}

	// Mapa
	Log().Get(TAG,logINFO) << "Cargando tile default";
	string tileDefault = "img/grass1n.png";
	Drawable *tileDefDrawable = new Drawable(64,0);
	if (tileDefDrawable -> loadTextureFromFile(tileDefault,this->sdlRenderer)){
		Log().Get(TAG,logINFO) << "Cargado tile default";
		this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>("tileDefault", tileDefDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	}

	// Cargar los tipos pasados por el YAML
	int i = 0;
	for (list<TipoConfig>::iterator it = tipos.begin(); it != tipos.end(); ++it) {
	  Log().Get(TAG,logDEBUG) << "Parseando tipo: " << i;
	  TipoConfig tipo = *it;

	  if((tipo.getNombre() == "") || (tipo.getImagen() == "")){
		  Log().Get(TAG,logERROR) << "Tipo N°" << i << " es incorrecto. Deberia tener nombre e imagen.";

	  } else {
		  Drawable *nodoDrawable = this->getDrawableFromTipoConfig(tipo);
		  bool textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getImagen(), this->sdlRenderer);
		  if(textureLoaded){
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " se cargo correctamente bajo el nombre " << tipo.getNombre();
			  this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>(tipo.getNombre(), nodoDrawable)
			  );
		  } else {
			  Log().Get(TAG,logINFO) << "Tipo N°" << i << " no se pudo cargar la imagen.";
		  }
	  }
	  i++;
	}
	return success;
}

Drawable* Renderer::getDrawableFromTipoConfig(TipoConfig tipo){
	if(tipo.getFPS() > 0){
		Log().Get(TAG,logINFO) << "Tipo " << tipo.getNombre() << " es un sprite.";
		Sprite *sprite = new Sprite(
			tipo.getPixelRefX(), tipo.getPixelRefY(),
			tipo.getAnchoFrame(), tipo.getAltoFrame(),
			tipo.getFPS(), tipo.getDelay()
		);
		return sprite;
	}

	Log().Get(TAG,logINFO) << "Tipo " << tipo.getNombre() << " es un drawable.";
	Drawable *drawable = new Drawable(
		tipo.getPixelRefX(), tipo.getPixelRefY()
	);
	return drawable;
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

// Start drawing from left to right scaning from top to bottom
bool drawOrder (pair<SDL_Point, Drawable*> first,pair<SDL_Point, Drawable*> second) {
	SDL_Point firstPoint = first.first;
	SDL_Point secondPoint = second.first;
	if((firstPoint.x + firstPoint.y) < (secondPoint.x + secondPoint.y)){
		return true;
	}
	return (firstPoint.x < secondPoint.x);
}

void Renderer::drawViews() {
	//Clear screen
	SDL_SetRenderDrawColor(this->sdlRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear(this->sdlRenderer);

	list<View*>::iterator i;
	for(i=this->views.begin(); i != this->views.end(); ++i) {
		View* view = *i;
		view->render(this);
	}

	// Order the views in the "paintor style" drawing
	this->drawablesToPaint.sort(drawOrder);

	list< pair<SDL_Point, Drawable*> >::iterator toPaint;
	for(toPaint = this->drawablesToPaint.begin(); toPaint != this->drawablesToPaint.end(); ++toPaint) {
		Drawable* drawable = toPaint->second;
		SDL_Point windowPoint = this->mapToWindowPoint(toPaint->first);
		SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);
		SDL_RenderCopy(sdlRenderer, drawable->getTexture(), drawable->getClipRect(), &renderQuad);
	}
	this->drawablesToPaint.clear();

	//Update screen
	SDL_RenderPresent(this->sdlRenderer);
}

SDL_Point Renderer::mapToWindowPoint(SDL_Point mapPoint){
	SDL_Point windowPoint = {0,0};
	// Cambio a coordenadas isometricas
	windowPoint.x = (mapPoint.x  - mapPoint.y);
	windowPoint.y = (mapPoint.x  + mapPoint.y) / 2;
	// Ajusta al punto de la pantalla
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
	// Cambio a coordenadas ventana
	mapPoint.x = centeredWindow.y + (centeredWindow.x / 2);
	mapPoint.y = centeredWindow.y - (centeredWindow.x / 2);
	return mapPoint;
}

SDL_Point Renderer::proyectedPoint(SDL_Point mapPoint, SDL_Point plano){
	int padding = 8; //para que no se vaya para afuera del mapa por abajo
	int x = mapPoint.x;
	int y = mapPoint.y;
	int width = plano.x;
	int height = plano.y;

	if (x < 0) {x = 0;}
	if (y < 0) {y = 0;}
	if (x > width) {x = width - padding;}
	if (y > height) {y = height - padding;}

	return {x,y};

}

void Renderer::draw(int mapPositionX, int mapPositionY, Drawable* drawable, bool iso) {
	SDL_Point mapRect = { mapPositionX, mapPositionY };
	SDL_Point windowPoint = this->mapToWindowPoint(mapRect);
	/*Log().Get("Renderer", logDEBUG) << " map: "<< " {" << mapPositionX <<"," << mapPositionY <<"}" << " window: "<< " {" << windowPoint.x <<"," << windowPoint.y <<"}";
	SDL_Point mapPoint = this->windowToMapPoint2(windowPoint);
	Log().Get("Renderer", logDEBUG) << " window: "<< " {" << windowPoint.x <<"," << windowPoint.y <<"}" << " window: "<< " {" << mapPoint.x <<"," << mapPoint.y <<"}";*/
	SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);

	if(this->isInsideWindow(&renderQuad)){
		//Log().Get(TAG,logDEBUG) << "Drawable inside window with rect { " << renderQuad.x << ", " << renderQuad.y << ", " << renderQuad.w << ", " << renderQuad.h << " }";
		// Only postpone drawing if its not the tiles
		if(this->drawablesByInstanceName.find("tileDefault")->second != drawable){
			this->drawablesToPaint.push_back(pair<SDL_Point, Drawable*>(mapRect, drawable));
		} else {
			SDL_RenderCopy(sdlRenderer, drawable->getTexture(), drawable->getClipRect(), &renderQuad);
		}
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
