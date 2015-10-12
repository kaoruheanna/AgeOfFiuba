/*
 * Renderer.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#include "Renderer.h"
#include "Sprite.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "EscenarioView.h"
#include "MapView.h"
#include "Menu/MiniEscenarioView.h"
#include "Menu/MiniMapView.h"
#include "Menu/MiniView.h"

const std::string TAG = "Renderer";

Renderer::Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos) {
	this->window = NULL;
	this->sdlRenderer = NULL;
	this->missingImageDrawable = NULL;
	this->textFont = NULL;
	this->escenarioView = NULL;
	this->miniEscenarioView = NULL;
	this->mainTilePosition = {screenWidth/2,0}; // para que el mapa este en la mitad
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->screenMenu = new ScreenMenu(this->screenWidth,MENU_HEIGHT);

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

	if( TTF_Init() == -1 ){
		Log().Get(TAG,logERROR) << "SDL_ttf no se pudo inicializar: "<<TTF_GetError();
		return false;
	}

	return true;
}

bool Renderer::loadMedia(list<TipoConfig> tipos) {
	bool success = true;

	// Imagen Default
	this->missingImageDrawable = new Drawable(TILE_WIDTH_PIXELS/2,0);
	success = this->missingImageDrawable->loadTextureFromFile("img/missingImage.png",this->sdlRenderer);
	if(!success){
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable default";
	}

	// Mapa
	Drawable *tileDefDrawable = new Drawable(TILE_WIDTH_PIXELS/2,0);
	if (tileDefDrawable -> loadTextureFromFile(TILE_DEFAULT_PATH,this->sdlRenderer)){
		this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>(TILE_DEFAULT_NAME, tileDefDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable del tile default";
	}

	Drawable *miniTileDefDrawable = new Drawable(TILE_WIDTH_PIXELS/2,0);
	if (miniTileDefDrawable->loadTextureFromFile(MINI_TILE_DEFAULT_PATH,this->sdlRenderer)){
		this->drawablesByInstanceName.insert(std::pair<std::string,Drawable*>(MINI_TILE_DEFAULT_NAME, miniTileDefDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable del tile default de minimap";
	}

	// Cargar los tipos pasados por el YAML
	int i = 0;
	for (list<TipoConfig>::iterator it = tipos.begin(); it != tipos.end(); ++it) {
	  TipoConfig tipo = *it;
	  if((tipo.getNombre() == "") || (tipo.getImagen() == "")){
		  Log().Get(TAG,logWARNING) << "Tipo N°" << i << " es incorrecto. Deberia tener nombre e imagen.";

	  } else {
		  Drawable *nodoDrawable = this->getDrawableFromTipoConfig(tipo);
		  bool textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getImagen(), this->sdlRenderer);
		  if(textureLoaded){
			  this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>(tipo.getNombre(), nodoDrawable)
			  );
		  } else {
			  Log().Get(TAG,logWARNING) << "Tipo N°" << i << " no se pudo cargar la imagen.";
		  }
	  }
	  i++;
	}

	this->textFont = TTF_OpenFont("img/arial.ttf", 16);
	if (this->textFont == NULL ) {
		Log().Get(TAG,logERROR) << "No se pudo cargar la fuente: "<< TTF_GetError();
		success = false;
	}

	return success;
}

Drawable* Renderer::getDrawableFromTipoConfig(TipoConfig tipo){
	if(tipo.getFPS() > 0){
		Sprite *sprite = new Sprite(
			tipo.getPixelRefX(), tipo.getPixelRefY(),
			tipo.getAnchoFrame(), tipo.getAltoFrame(),
			tipo.getFPS(), tipo.getDelay()
		);
		return sprite;
	}

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

	// delete menu
	TTF_CloseFont(this->textFont);
	this->textFont = NULL;
	delete this->screenMenu;

	//Destroy window
	SDL_DestroyRenderer(this->sdlRenderer);
	SDL_DestroyWindow(this->window);
	this->window = NULL;
	this->sdlRenderer = NULL;
	this->escenarioView = NULL;

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

	this->drawEscenario();
	this->drawMenu();
	this->drawMiniEscenario();

	//Update screen
	SDL_RenderPresent(this->sdlRenderer);
}

void Renderer::drawEscenario() {
	//set viewport
	SDL_Rect viewportRect;
	viewportRect.x = 0;
	viewportRect.y = 0;
	viewportRect.w = this->screenWidth;
	viewportRect.h = this->menuOriginY();
	SDL_RenderSetViewport(this->sdlRenderer, &viewportRect);

	//Clear screen
	SDL_SetRenderDrawColor(this->sdlRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear(this->sdlRenderer);

	this->escenarioView->render(this);

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
}

void Renderer::drawMenu(){
	SDL_Rect viewportRect;
	viewportRect.x = 0;
	viewportRect.y = this->menuOriginY();
	viewportRect.w = this->screenWidth;
	viewportRect.h = MENU_HEIGHT;
	SDL_RenderSetViewport(this->sdlRenderer, &viewportRect);
	this->screenMenu->render(this);
}

void Renderer::drawMiniEscenario(){
	int height = MENU_HEIGHT-(2*MENU_SPACING);
	int width = height * 2;
	int x = (this->screenWidth - MENU_SPACING - width);
	int y = (this->menuOriginY() +  MENU_SPACING);
	this->miniMapMainTilePosition = {width/2,0};

	SDL_Rect viewportRect;
	viewportRect.x = x;
	viewportRect.y = y;
	viewportRect.w = width;
	viewportRect.h = height;
	SDL_RenderSetViewport(this->sdlRenderer, &viewportRect);

	SDL_Rect rect = {0,0,width,height};
	SDL_Color color = {0, 0, 0};
	this->draw(rect,color);

	this->miniEscenarioView->render(this);
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

// draw Drawable
void Renderer::draw(int mapPositionX, int mapPositionY, Drawable* drawable) {
	SDL_Point mapRect = { mapPositionX, mapPositionY };
	SDL_Point windowPoint = this->mapToWindowPoint(mapRect);
	SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);

	if(this->isInsideWindow(&renderQuad)){
		// Only postpone drawing if its not the tiles
		if(this->drawablesByInstanceName.find(TILE_DEFAULT_NAME)->second != drawable){
			this->drawablesToPaint.push_back(pair<SDL_Point, Drawable*>(mapRect, drawable));
		} else {
			SDL_RenderCopy(sdlRenderer, drawable->getTexture(), drawable->getClipRect(), &renderQuad);
		}
	}
}

// draw shape
void Renderer::draw(SDL_Rect rect, SDL_Color color){
	SDL_SetRenderDrawColor(this->sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(this->sdlRenderer, &rect);
}

void Renderer::drawTextureInRect(SDL_Texture *texture,SDL_Rect rect){
	SDL_RenderCopy(this->sdlRenderer, texture, NULL, &rect);
}

bool Renderer::isInsideWindow(SDL_Rect* rect){
	int maxY = this->menuOriginY();

	return (rect->x < this->screenWidth &&
			rect->x + rect->w > 0 &&
		    rect->y < maxY &&
			rect->y + rect->h > 0);
}

bool Renderer::canDraw() {
	return this->successfullInit;
}

SDL_Renderer* Renderer::getSdlRenderer(){
	return this->sdlRenderer;
}

TTF_Font* Renderer::getFont(){
	return this->textFont;
}

int Renderer::menuOriginY(){
	return (this->screenHeight - MENU_HEIGHT);
}

SDL_Point Renderer::escenarioSize(){
	int height = this->menuOriginY();
	return {this->screenWidth,height};
}

void Renderer::setEscenarioView(EscenarioView *escenarioView){
	this->escenarioView = escenarioView;
	this->updatedEscenario();
}

void Renderer::updatedEscenario(){
	MapView *mapView = this->escenarioView->getMapView();
	this->setDrawableForView(mapView);

	list<View*>* entitiesViews = this->escenarioView->getEntitiesView();
	list<View*>::iterator i;
	for(i=entitiesViews->begin(); i != entitiesViews->end(); ++i) {
		View* view = *i;
		this->setDrawableForView(view);
	}
}

void Renderer::setDrawableForView(View* view){
	std::map<std::string,Drawable *>::iterator found = this->drawablesByInstanceName.find(view->getType());
	Drawable* drawable = NULL;
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	} else {
		Log().Get(TAG,logWARNING) << "No se pudo cargar la imagen: '"<<view->getType().c_str()<<"', usa la imagen default";
		drawable = this->missingImageDrawable;
	}
	view->setDrawable(drawable);
}

// MINIMAP
void Renderer::setMiniEscenarioView(MiniEscenarioView *miniEscenarioView){
	this->miniEscenarioView = miniEscenarioView;
	this->updatedMiniEscenario();
}

void Renderer::updatedMiniEscenario(){
	MiniMapView *miniMapView = this->miniEscenarioView->getMiniMapView();
	this->setDrawableForMiniView(miniMapView);
}

void Renderer::setDrawableForMiniView(MiniView* view){
	std::map<std::string,Drawable *>::iterator found = this->drawablesByInstanceName.find(view->getType());
	Drawable* drawable = NULL;
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	} else {
		Log().Get(TAG,logWARNING) << "No se pudo cargar la imagen: '"<<view->getType().c_str()<<"', usa la imagen default";
		drawable = this->missingImageDrawable;
	}
	view->setDrawable(drawable);
}

void Renderer::drawInMiniMap(int mapPositionX, int mapPositionY, Drawable* drawable) {
	SDL_Point windowPoint = {0,0};
	// Cambio a coordenadas isometricas
	windowPoint.x = (mapPositionX  - mapPositionY);
	windowPoint.y = (mapPositionX  + mapPositionY) / 2;

	float mapHeight = (float)(this->escenarioView->getMapView()->getHeightInPixels());
	float miniMapHeight = (float)(MENU_HEIGHT-(2*MENU_SPACING));
	float factor = (miniMapHeight/mapHeight);

	SDL_Rect originalQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);
	SDL_Rect renderQuad;
	renderQuad.x = (originalQuad.x * factor) + this->miniMapMainTilePosition.x;
	renderQuad.y = (originalQuad.y * factor) + this->miniMapMainTilePosition.y;
	renderQuad.w = (originalQuad.w * factor);
	renderQuad.h = (originalQuad.h * factor);

	SDL_RenderCopy(sdlRenderer, drawable->getTexture(), NULL, &renderQuad);
}



