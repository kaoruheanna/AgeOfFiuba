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
#include "TopBar/TopBar.h"
#include "Cartel/Cartel.h"

const std::string TAG = "Renderer";

Renderer::Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos) {
	this->window = NULL;
	this->sdlRenderer = NULL;
	this->missingImageDrawable = NULL;
	this->miniMissingImageDrawable = NULL;
	this->textFont = NULL;
	this->escenarioView = NULL;
	this->miniEscenarioView = NULL;
	this->mainTilePosition = {screenWidth/2,0}; // para que el mapa este en la mitad
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->screenMenu = new ScreenMenu(this->screenWidth,MENU_HEIGHT);
	this->topBar = new TopBar(this->screenWidth,TOP_BAR_HEIGHT);
	this->fog = NULL;
	this->hasSelectedTiles = false;
	this->cartel = NULL;
	this->selectionArea = {0,0,0,0};
	//this->selectedEntity = NULL;

	bool didInitSDL = this->initSDL();
	bool didLoadMedia = this->loadMedia(tipos);
	this->successfullInit = (didInitSDL && didLoadMedia);
	this->initRects();
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
	success = this->missingImageDrawable->loadTextureFromFile(MISSING_IMAGE_PATH,this->sdlRenderer);
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

	// Cargar los tipos pasados por el YAML
	int i = 0;
	for (list<TipoConfig>::iterator it = tipos.begin(); it != tipos.end(); ++it) {
	  TipoConfig tipo = *it;
	  if((tipo.getNombre() == "") || (tipo.getImagen() == "")){
		  Log().Get(TAG,logWARNING) << "Tipo N°" << i << " es incorrecto. Deberia tener nombre e imagen.";

	  } else {
		  // Cargar textura comun
		  Drawable *nodoDrawable = this->getDrawableFromTipoConfig(tipo);
		  bool textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getImagen(), this->sdlRenderer);
		  if(textureLoaded){
			  this->drawablesByInstanceName.insert(
				std::pair<std::string,Drawable*>(tipo.getNombre(), nodoDrawable)
			  );
		  } else {
			  Log().Get(TAG,logWARNING) << "Tipo N°" << i << " no se pudo cargar la imagen.";
		  }
		  // Cargar textura deshabilitado
		  if(tipo.getImagenDeshabilitado().compare(tipo.getImagen()) != 0){
			  nodoDrawable = this->getDrawableFromTipoConfig(tipo);
			  textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getImagenDeshabilitado(), this->sdlRenderer);
			  if(textureLoaded){
				  this->drawablesByInstanceName.insert(
					std::pair<std::string,Drawable*>(tipo.getNombre() + "-deshabilitado", nodoDrawable)
				  );
			  } else {
				  Log().Get(TAG,logWARNING) << "Tipo N°" << i << " no se pudo cargar la imagen.";
			  }
		  }

		  // boton
		  if (tipo.getImagenBoton() != ""){
			  Drawable *nodoDrawable = new Drawable(0,0);
			  bool textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getImagenBoton(), this->sdlRenderer);
			  if(textureLoaded){
				this->drawablesByInstanceName.insert(std::pair<std::string,Drawable*>("boton-"+tipo.getNombre(), nodoDrawable));
			  }
		  }
	  }
	  i++;
	}

	bool minimapSuccess = this->loadMediaForMiniMap(&tipos);

	this->textFont = TTF_OpenFont("img/arial.ttf", 16);
	if (this->textFont == NULL ) {
		Log().Get(TAG,logERROR) << "No se pudo cargar la fuente: "<< TTF_GetError();
		success = false;
	}

	return (success & minimapSuccess);
}

bool Renderer::loadMediaForMiniMap(list<TipoConfig>* tipos){
	int pixelRefX = TILE_WIDTH_PIXELS/2;
	int pixelRefY = 0;

	//imagen default
	this->miniMissingImageDrawable = new Drawable(pixelRefX,pixelRefY);
	bool success = this->miniMissingImageDrawable->loadTextureFromFile(MINI_MISSING_IMAGE_PATH,this->sdlRenderer);
	if(!success){
		Log().Get(TAG,logERROR) << "No se pudo cargar el mini drawable default";
		return false;
	}

	Drawable *miniTileDefDrawable = new Drawable(pixelRefX,pixelRefY);
	if (miniTileDefDrawable->loadTextureFromFile(MINI_TILE_DEFAULT_PATH,this->sdlRenderer)){
		this->drawablesByInstanceName.insert(std::pair<std::string,Drawable*>(MINI_TILE_DEFAULT_NAME, miniTileDefDrawable));
	} else {
		Log().Get(TAG,logERROR) << "No se pudo cargar el drawable del tile default de minimap";
		return false;
	}

	for (list<TipoConfig>::iterator it = tipos->begin(); it != tipos->end(); ++it) {
		TipoConfig tipo = *it;
		if((tipo.getNombre() != "") && (tipo.getMiniImagen() != "")){
			string name = MiniView::NombreDrawableFromNombreTipo(tipo.getNombre());

			int pixelRefType = tipo.getMinimapPixelRefType();
			int x = (pixelRefType == MinimapPixelRefTypeCentered) ? (TILE_WIDTH_PIXELS/2) : pixelRefX;
			int y = (pixelRefType == MinimapPixelRefTypeCentered) ? (TILE_HEIGHT_PIXELS/2) : pixelRefY;

			Drawable *nodoDrawable = new Drawable(x,y);
			bool textureLoaded = nodoDrawable->loadTextureFromFile(tipo.getMiniImagen(), this->sdlRenderer);
			if(textureLoaded){
				this->drawablesByInstanceName.insert(std::pair<std::string,Drawable*>(name, nodoDrawable));
			}
		}
	}
	return true;
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

	this->fog->close();
	delete this->fog;
	this->fog = NULL;

	this->missingImageDrawable->free();
	delete this->missingImageDrawable;

	this->miniMissingImageDrawable->free();
	delete this->miniMissingImageDrawable;

	// delete menu
	TTF_CloseFont(this->textFont);
	this->textFont = NULL;
	delete this->screenMenu;
	delete this->topBar;

	if (this->cartel){
		delete this->cartel;
	}

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
bool drawOrder (pair<SDL_Point, DrawableWithState> first,pair<SDL_Point, DrawableWithState> second) {
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

	this->drawTopBar();
	this->drawEscenario();
	this->drawSelectionRect();
	this->drawMenu();
	this->drawMiniEscenario();

	//Update screen
	SDL_RenderPresent(this->sdlRenderer);
}

void Renderer::drawTopBar() {
	SDL_RenderSetViewport(this->sdlRenderer, &this->topBarRect);
	this->topBar->render(this);
}

void Renderer::drawEscenario() {
	//set viewport
	SDL_RenderSetViewport(this->sdlRenderer, &this->escenarioRect);

	this->escenarioView->render(this);

	// Order the views in the "paintor style" drawing
	this->drawablesToPaint.sort(drawOrder);

	list< pair<SDL_Point, DrawableWithState> >::iterator toPaint;
	for(toPaint = this->drawablesToPaint.begin(); toPaint != this->drawablesToPaint.end(); ++toPaint) {
		Drawable* drawable = toPaint->second.drawable;
		SDL_Point windowPoint = this->mapToWindowPoint(toPaint->first);
		SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);
		SDL_Rect* clipRect = NULL;
		if(toPaint->second.hasRect){
			clipRect = &toPaint->second.rect;
		}

		SDL_Point currentTile = { toPaint->first.x / TILE_HEIGHT_PIXELS, toPaint->first.y / TILE_HEIGHT_PIXELS };
		EstadoDeVisibilidad currentTileState = this->fog->getEstado(currentTile.x,currentTile.y);

		if (currentTileState != OCULTO){
			if (currentTileState == VISIBLE){
					SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISIBLE,FOG_VISIBLE,FOG_VISIBLE );
			} else if (currentTileState == NUBLADO) {
					SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISITED,FOG_VISITED,FOG_VISITED );
			}

			SDL_RenderCopy(sdlRenderer, drawable->getTexture(), clipRect, &renderQuad);
		}
	}
	this->drawablesToPaint.clear();

	this->drawCartelIfShould();
}

void Renderer::drawCartelIfShould(){
	if (this->cartel){
		this->cartel->render(this);
	}
}

void Renderer::drawMenu(){
	SDL_RenderSetViewport(this->sdlRenderer, &this->menuRect);
	//TODO ver como hacer para dibujar la base del menu sin una selected entity
	this->screenMenu->renderMenu(this);
	this->screenMenu->renderMenuForEntity(this,this->selectedEntities);
}

void Renderer::drawMiniEscenario(){
	this->miniMapMainTilePosition = {(this->minimapRect.w/2),0};
	SDL_RenderSetViewport(this->sdlRenderer, &this->minimapRect);

	SDL_Rect rect = {0,0,this->minimapRect.w,this->minimapRect.h};
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

void Renderer::setearColor(Drawable* drawable){
	//TODO si selectedEntities esta vacio?
	switch (this->selectedEntities.front()->getTeam()){
		case TEAM_RED:
			SDL_SetTextureColorMod( drawable->getTexture(), 255,0,0 );
			break;
		case TEAM_BLUE:
			SDL_SetTextureColorMod( drawable->getTexture(), 0,0,255 );
			break;
		case TEAM_GREEN:
			SDL_SetTextureColorMod( drawable->getTexture(), 0,255,0 );
			break;
		case TEAM_YELLOW:
			SDL_SetTextureColorMod( drawable->getTexture(), 255,255,0 );
			break;
		default:
			SDL_SetTextureColorMod( drawable->getTexture(), 200,200,200 );
			break;
	}
}

bool Renderer::sonTilesIguales(int tileX, int tileY, int selectedTileX, int selectedTileY){
	return ((selectedTileX == tileX)&&(selectedTileY == tileY));
}

// draw Drawable
void Renderer::draw(int mapPositionX, int mapPositionY, Drawable* drawable,bool admiteNublado) {
	SDL_Point mapRect = { mapPositionX, mapPositionY };
	SDL_Point windowPoint = this->mapToWindowPoint(mapRect);
	SDL_Rect renderQuad = drawable->getRectToDraw(windowPoint.x, windowPoint.y);

	if(!(this->isInsideWindow(&renderQuad))){
		//como no esta dentro de la ventana, no lo dibuja
		return;
	}

	SDL_Point currentTile = { mapPositionX / TILE_HEIGHT_PIXELS, mapPositionY / TILE_HEIGHT_PIXELS };
	EstadoDeVisibilidad currentTileState = this->fog->getEstado(currentTile.x,currentTile.y);

	if (currentTileState == OCULTO){
		// como todavia esta oculto no lo dibujo
		return;
	}

	if((currentTileState == NUBLADO) && !admiteNublado){
		return;
	}

	if(this->drawablesByInstanceName.find(TILE_DEFAULT_NAME)->second != drawable){
		// si no es un tile, lo guarda para dibujar despues
		DrawableWithState state;
		state.drawable = drawable;
		state.hasRect = (drawable->getClipRect() != NULL);
		if(state.hasRect){
			state.rect = *drawable->getClipRect();
		}
		this->drawablesToPaint.push_back(pair<SDL_Point, DrawableWithState>(mapRect, state));
		return;
	}

	if (currentTileState == VISIBLE){
		SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISIBLE,FOG_VISIBLE,FOG_VISIBLE );
	} else if (currentTileState == NUBLADO) {
		SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISITED,FOG_VISITED,FOG_VISITED );
	}
	if (!this->selectedEntities.empty()){
		for (Entity* entity: this->selectedEntities){
			if (entity->getClass() == MOBILE_MODEL && currentTileState != NUBLADO  ){
				int tileX = currentTile.x;
				int	tileY = currentTile.y;
				int selectedTileX = entity->getPosicion().x / TILE_HEIGHT_PIXELS;
				int selectedTileY = entity->getPosicion().y / TILE_HEIGHT_PIXELS;
				if (this->sonTilesIguales(tileX,tileY,selectedTileX,selectedTileY)){
					this->setearColor(drawable);
				}
			}
			//si es un tile lo dibuja ahora
			if (this->hasSelectedTiles && entity->getClass() != MOBILE_MODEL){
				int minX = this->selectedTilesCoordinates.first.x;
				int maxX = this->selectedTilesCoordinates.second.x;
				int minY = this->selectedTilesCoordinates.first.y;
				int maxY = this->selectedTilesCoordinates.second.y;
				bool inRangeX = ((currentTile.x >= minX) && (currentTile.x < maxX));
				bool inRangeY = ((currentTile.y >= minY) && (currentTile.y < maxY));
				if (inRangeX && inRangeY){
					this->setearColor(drawable);
				}
			}
		}
	}

	SDL_RenderCopy(sdlRenderer, drawable->getTexture(), drawable->getClipRect(), &renderQuad);
}

// draw shape
void Renderer::draw(SDL_Rect rect, SDL_Color color){
	SDL_SetRenderDrawColor(this->sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(this->sdlRenderer, &rect);
}

void Renderer::drawTextureInRect(SDL_Texture *texture,SDL_Rect rect){
	SDL_RenderCopy(this->sdlRenderer, texture, NULL, &rect);
}

void Renderer::drawActionButtonWithNameInRect(string name, SDL_Rect rect){
	std::map<std::string,Drawable *>::iterator found = this->drawablesByInstanceName.find(name);
	Drawable* drawable = NULL;
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	} else {
		drawable = this->missingImageDrawable;
	}
	SDL_RenderCopy(this->sdlRenderer, drawable->getTexture(), NULL, &rect);
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
	int height = this->menuOriginY() - TOP_BAR_HEIGHT;
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
	// Find deshabilited drawable
	found = this->drawablesByInstanceName.find(view->getType() + "-deshabilitado");
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	}
	view->setDrawableDeshabilitado(drawable);
}

// MINIMAP
void Renderer::setMiniEscenarioView(MiniEscenarioView *miniEscenarioView){
	this->miniEscenarioView = miniEscenarioView;
	this->updatedMiniEscenario();
}

void Renderer::updatedMiniEscenario(){
	MiniMapView *miniMapView = this->miniEscenarioView->getMiniMapView();
	this->setDrawableForMiniView(miniMapView);

	list<MiniView*>* entitiesMiniViews = this->miniEscenarioView->getEntitiesMiniView();
	list<MiniView*>::iterator i;
	for(i=entitiesMiniViews->begin(); i != entitiesMiniViews->end(); ++i) {
		MiniView* view = *i;
		this->setDrawableForMiniView(view);
	}
}

void Renderer::setDrawableForMiniView(MiniView* view){
	std::map<std::string,Drawable *>::iterator found = this->drawablesByInstanceName.find(view->getType());
	Drawable* drawable = NULL;
	if(found != this->drawablesByInstanceName.end()){
		drawable = found->second;
	} else {
		Log().Get(TAG,logWARNING) << "No se pudo cargar la imagen: '"<<view->getType().c_str()<<"', usa la imagen default";
		drawable = this->miniMissingImageDrawable;
	}
	view->setDrawable(drawable);
}

void Renderer::drawInMiniMap(int mapPositionX, int mapPositionY, Drawable* drawable, bool admiteNublado) {
	SDL_Point windowPoint = {0,0};

	SDL_Point currentTile = { mapPositionX / TILE_HEIGHT_PIXELS, mapPositionY / TILE_HEIGHT_PIXELS };
	EstadoDeVisibilidad currentTileState = this->fog->getEstado(currentTile.x,currentTile.y);

	if (currentTileState == OCULTO){return;}

	if (currentTileState == NUBLADO && !admiteNublado){
		return;
	}

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


	if(currentTileState == VISIBLE) SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISIBLE,FOG_VISIBLE,FOG_VISIBLE );
	else if(currentTileState == NUBLADO) SDL_SetTextureColorMod( drawable->getTexture(), FOG_VISITED,FOG_VISITED,FOG_VISITED );


	SDL_RenderCopy(sdlRenderer, drawable->getTexture(), NULL, &renderQuad);

}

void Renderer::setFog(int ancho,int alto){
	this->fog = new FogOfWar(ancho,alto);
}

void Renderer::fogUpdate(list<Entity*> entidades, Team equipo){
	this->fog->update(entidades, equipo);
}

FogOfWar* Renderer::getFog(){
	return this->fog;
}

void Renderer::setProtagonista(User *protagonista) {
	this->topBar->setProtagonista(protagonista);
}

void Renderer::setMessagesInMenu(std::string firstMessage, std::string secondMessage) {
	this->screenMenu->setMessages(firstMessage,secondMessage);
}

void Renderer::setSelectedTilesCoordinates(bool selected,std::list<pair<SDL_Point,SDL_Point>> tiles, list<Entity*> entidad){
	//TODO hay que arreglar esto para que funcione para todas las unidades seleccionadas.
	this->hasSelectedTiles = selected;
	this->selectedTilesCoordinates = tiles.front();
	this->selectedEntity = entidad.front();
}

void Renderer::setCartel(string message){
	if (!this->cartel){
		int x = (this->screenWidth - CARTEL_WIDTH) / 2;
		int y = 100;
		this->cartel = new Cartel(x,y);
	}
	this->cartel->setMessage(message);
}

void Renderer::hideCartel(){
	if (this->cartel){
		delete this->cartel;
		this->cartel = NULL;
	}
}

void Renderer::initRects(){
	//top bar
	this->topBarRect.x = 0;
	this->topBarRect.y = 0;
	this->topBarRect.w = this->screenWidth;
	this->topBarRect.h = TOP_BAR_HEIGHT;

	// escenario
	this->escenarioRect.x = 0;
	this->escenarioRect.y = (this->topBarRect.y + this->topBarRect.h);
	this->escenarioRect.w = this->screenWidth;
	this->escenarioRect.h = this->menuOriginY();

	// menu
	this->menuRect.x = 0;
	this->menuRect.y = this->menuOriginY();
	this->menuRect.w = this->screenWidth;
	this->menuRect.h = MENU_HEIGHT;

	// minimap
	this->minimapRect.h = MENU_HEIGHT-(2*MENU_SPACING);
	this->minimapRect.w = this->minimapRect.h * 2;
	this->minimapRect.x = (this->screenWidth - MENU_SPACING - this->minimapRect.w);
	this->minimapRect.y = (this->menuOriginY() +  MENU_SPACING);
}

void Renderer::clickEvent(int x, int y, bool leftClick, RendererInteractionDelegate *delegate) {
	if (this->isPixelInEscenario(x,y)){
		if (leftClick){
			delegate->leftClickEnEscenario(x,y);
		} else {
			delegate->rightClickEnEscenario(x,y);
		}
		return;
	}

	if (this->isPixelInMenu(x,y)){
		//le resto el offset del viewport
		int xMenu = x - this->menuRect.x;
		int yMenu = y - this->menuRect.y;
		this->screenMenu->clickEvent(xMenu,yMenu,delegate);
	}

}

void Renderer::dragLeftClickEvent(int xi, int yi, int xf, int yf){
	this->selectionArea.x = xi;
	this->selectionArea.y = yi;
	this->selectionArea.w = xf-xi;
	this->selectionArea.h = yf-yi;
}

void Renderer::leftMouseUpEvent(RendererInteractionDelegate *delegate){
	delegate->leftMouseUp(this->selectionArea.x, this->selectionArea.y,
			this->selectionArea.w, this->selectionArea.h);
	this->selectionArea = {0,0,0,0};

}

bool Renderer::isPixelInEscenario(int x, int y){
	return this->isPixelInRect(x,y,this->escenarioRect);
}

bool Renderer::isPixelInMenu(int x, int y){
	return this->isPixelInRect(x,y,this->menuRect);
}

bool Renderer::isPixelInRect(int x, int y, SDL_Rect rect){
	if (x < rect.x){
		return false;
	}
	if (x > (rect.x + rect.w)){
		return false;
	}
	if (y < rect.y){
		return false;
	}
	if (y > (rect.y + rect.h)){
		return false;
	}
	return true;
}

void Renderer::drawSelectionRect(){
	SDL_SetRenderDrawColor( this->sdlRenderer, 0xFF, 0xFF, 0xFF, 0x00 );
	SDL_RenderDrawRect( this->sdlRenderer, &this->selectionArea);
}
