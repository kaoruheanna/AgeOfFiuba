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
#include "../Models/Escenario.h"
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

void GameController::agregarEntidades(list<Entity*> entidades) {
	list<Entity*>::iterator entidad;
	int indice = 0;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		if (entidadReal != this->escenario->getProtagonista()) {
			EntityView* entityView = new EntityView(entidadReal->getNombre());
			entityView->setModel(entidadReal);
			this->views.push_back(entityView);
			this->renderer->addView(entityView);
		}
		indice++;
	}
}

bool GameController::play() {
	this->renderer = new Renderer(this->config->getPantallaAncho(),this->config->getPantallaAlto(), this->config->getTipos());
	if (!this->renderer->canDraw()){
		Log().Get(TAG,logERROR) << "Failed to initialize Renderer!";
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
	list<Entity*> entidades = escenario->getListaEntidades();
	agregarEntidades(entidades);

	// Agrego vista del personaje
	MobileView *marioView = new MobileView(this->escenario->getProtagonista()->getNombre());
	marioView->setModel(this->escenario->getProtagonista());
	this->views.push_back(marioView);
	this->renderer->addView(marioView);

	initWindowSizes();

	//Inicializar resources Manager
	this->resourcesManager = new ResourcesManager(config->getTamanioX(),config->getTamanioY());

	bool shouldRestart = false;
	//While application is running
	while( !this->shouldQuit && !shouldRestart ) {
		this->updateWindow();
		shouldRestart = this->pollEvents();
		this->escenario->getProtagonista()->updatePosition();
		this->renderer->drawViews();
		this->sleep();
		agregarEntidades(this->resourcesManager->InsertResourcesForNewLoopOnMap());
	}

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

void GameController::initWindowSizes() {
	SDL_Point intialWindowWrapperTop;
	SDL_Point intialWindowWrapperBottom;
	SDL_Point intialWindowWrapperLeft;
	SDL_Point intialWindowWrapperRight;

	SDL_Point pointL;
	pointL.x = 0;
	pointL.y = 0;
	intialWindowWrapperTop = this->renderer->mapToWindowPoint(pointL);


	pointL.x = this->escenario->getSize().x;
	pointL.y = 0;
	intialWindowWrapperRight = this->renderer->mapToWindowPoint(pointL);

	pointL.x = 0;
	pointL.y = this->escenario->getSize().y;
	intialWindowWrapperLeft = this->renderer->mapToWindowPoint(pointL);


	pointL = this->escenario->getSize();
	intialWindowWrapperBottom = this->renderer->mapToWindowPoint(pointL);

	vertixSlope = -1 * (intialWindowWrapperTop.x - intialWindowWrapperLeft.x) / (intialWindowWrapperTop.y - intialWindowWrapperLeft.y);
	middlePoint = -1 * intialWindowWrapperLeft.y;

	intialPointWindowWrapper.x = -intialWindowWrapperLeft.x + (this->config->getPantallaAncho()/2);
	intialPointWindowWrapper.y = -intialWindowWrapperTop.y;

	finalPointWindowWrapper.x = -intialWindowWrapperRight.x;
	finalPointWindowWrapper.y = -intialWindowWrapperBottom.y;
}

float GameController::scrollingSpeedX(int x) {
	return scrollingSpeed(x,this->config->getPantallaAncho())*-1;
}

float GameController::scrollingSpeedY(int y) {
	return scrollingSpeed(y,this->config->getPantallaAlto())*-1;
}

SDL_Point GameController::getMaxVertixForPoint(int yPosition) {
 	SDL_Point maxVertix = { 0, 0 };
	if(yPosition < middlePoint){
		// If is the bottom half of the map then the slope is inverted
		int invertedYPosition = 2* middlePoint - yPosition;
		maxVertix.x = vertixSlope * invertedYPosition + this->config->getPantallaAncho();
		maxVertix.y = -1 * vertixSlope * invertedYPosition;
	} else {
		maxVertix.x = vertixSlope * yPosition;
		maxVertix.y = this->config->getPantallaAncho() - vertixSlope * yPosition;
	}
	return maxVertix;
}

void GameController::moveToPoint(SDL_Point point) {
	// Checkea que el scroll no se vaya por las perpendiculares
	point.y = (point.y > intialPointWindowWrapper.y) ? intialPointWindowWrapper.y :point.y;
	point.y = (point.y < (finalPointWindowWrapper.y + this->config->getPantallaAlto())) ? (finalPointWindowWrapper.y + this->config->getPantallaAlto()) : point.y;
	point.x = (point.x > intialPointWindowWrapper.x) ? intialPointWindowWrapper.x : point.x;
	point.x = (point.x < (finalPointWindowWrapper.x + (1.5*this->config->getPantallaAncho()))) ? (finalPointWindowWrapper.x + (1.5*this->config->getPantallaAncho())) :point.x;

	// Checkea que el scroll no se vaya por las diagonales
	SDL_Point maxVertix = this->getMaxVertixForPoint(point.y);
	point.x = (point.x < maxVertix.x)? maxVertix.x: point.x;
	point.x = (point.x > maxVertix.y)? maxVertix.y: point.x;

	this->renderer->mainTilePosition = point;
}

void GameController::updateWindow() {
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
			this->shouldQuit = true;
		}

		if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
			this->shouldQuit = true;
			pressedR = true;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN){
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			SDL_Point point = this->renderer->windowToMapPoint({x,y});

			point = this->renderer->proyectedPoint(point, this->escenario->getSize());

			this->escenario->getProtagonista()->setDestination(point.x,point.y);
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
		View* view = *i;
		delete view;
	}
	this->views.clear();
}

void GameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}
