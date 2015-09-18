/*
 * GameController.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#include "GameController.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "../Views/MobileView.h"
#include "../Views/EntityView.h"
#include "../Views/MapView.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

const std::string TAG = "GameController";

GameController::GameController(GameConfiguration *config) {
	this->shouldQuit = false;
	this->renderer = NULL;
	this->config = config;
	this->escenario = NULL;
}

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}

bool GameController::play() {
	Log().Get(TAG,logDEBUG) << "[GameController] " << "play";

	this->renderer = new Renderer(this->config->getPantallaAncho(),this->config->getPantallaAlto(), this->config->getTipos());
	if (!this->renderer->canDraw()){
		printf( "Failed to initialize!\n" );
		this->close();
		return false;
	}

	// Crear modelos a partir de la configuracion
	this->escenario = new Escenario(this->config->getEscenario(), this->config->getTipos());
	if(!this->escenario->inicializacionCorrecta){
		delete this->escenario;
		delete this->config;
		// Cargar configuracion default
		this->config = new GameConfiguration();
		this->escenario = new Escenario(this->config->getEscenario(), this->config->getTipos());
		if(!this->escenario->inicializacionCorrecta){
			Log().Get("GameController", logERROR) << "La configuracion default es incorrecta.";
			this->close();
			return false;
		}
	}

	// Crear vistas a partir de la configuracion
	MapView *mapView = new MapView("tileDefault");
	mapView->setModel(this->escenario->mundo);
	this->views.push_back(mapView);
	this->renderer->addView(mapView);

	// Agrego todas las vistas (siempre que no sean el protagonista)
	list<Entity*>::iterator entidad;
	list<Entity*> entidades = escenario->getListaEntidades();
	int indice = 0;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad){
		Entity* entidadReal = (*entidad);
		printf("indice: %i %s \n", indice, entidadReal->getNombre().c_str());
		if(entidadReal != this->escenario->getProtagonista()){
			EntityView* entityView = new EntityView(entidadReal->getNombre());
			entityView->setModel(entidadReal);
			this->views.push_back(entityView);
			this->renderer->addView(entityView);
		}
		indice++;
	}
	printf("created views");
	// Agrego vista del personaje
	MobileView *marioView = new MobileView(this->escenario->getProtagonista()->getNombre());
	marioView->setModel(this->escenario->getProtagonista());
	this->views.push_back(marioView);
	this->renderer->addView(marioView);

	printf("created player");
	bool shouldRestart = false;
	//While application is running
	while( !this->shouldQuit && !shouldRestart ) {
		this->updateWindow();
		shouldRestart = this->pollEvents();
		this->escenario->getProtagonista()->updatePosition();
		this->renderer->drawViews();
		this->sleep();
	}
	printf("closing controller");
	this->close();
	return shouldRestart;
}


float GameController::scrollingSpeed(int x, int large) {
	if ((x < this->config->getMargenScroll()) && (x>0)) {
		return ((float)(x-this->config->getMargenScroll())/this->config->getMargenScroll());
	}
	if ((x > (large - this->config->getMargenScroll())) && (x < large)) {
		return ((float)(x - large + this->config->getMargenScroll())/this->config->getMargenScroll());
	}
	return 0;
}

float GameController::scrollingSpeedX(int x) {
	return scrollingSpeed(x,this->config->getPantallaAncho())*-1;
}

float GameController::scrollingSpeedY(int y) {
	return scrollingSpeed(y,this->config->getPantallaAlto())*-1;
}

void GameController::moveToPoint(SDL_Point point) {
	this->renderer->mainTilePosition = point;
}

void GameController::updateWindow() {
	const int SCROLL_SPEED = 10;

	//Get mouse position
	int x, y, newY, newX;
	SDL_GetMouseState(&x, &y);

	newX = (scrollingSpeedX(x)*SCROLL_SPEED) + this->renderer->mainTilePosition.x;
	newY = (scrollingSpeedY(y)*SCROLL_SPEED) + this->renderer->mainTilePosition.y;

	if ((newX != this->renderer->mainTilePosition.x) || (newY != this->renderer->mainTilePosition.y)) {
		moveToPoint({newX,newY});
	}
}

bool GameController::pollEvents(){
	bool pressedR = false;
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) {

		if( e.type == SDL_QUIT ) {
			printf("tengo que cerrarlo \n");
			this->shouldQuit = true;
		}

		if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
			printf("tengo que reiniciarlo \n");
			this->shouldQuit = true;
			pressedR = true;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN){
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			SDL_Point mapPoint = this->renderer->windowToMapPoint({x,y});
			this->escenario->getProtagonista()->setDestination(mapPoint.x,mapPoint.y);
		}
	}
	return pressedR;
}

void GameController::close() {
	if (this->renderer){
		this->renderer->close();
		delete this->renderer;
	}

	list<View*>::iterator i;
	for(i=this->views.begin(); i != this->views.end(); ++i) {
		printf("limpio una vista");
		View* view = *i;
		delete view;
	}
	this->views.clear();
}

void GameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}
