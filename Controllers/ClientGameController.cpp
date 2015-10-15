/*
 * ClientGameController.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#include "ClientGameController.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "../Views/MobileView.h"
#include "../Views/EntityView.h"
#include "../Views/MapView.h"
#include "../Models/Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "../Views/EscenarioView.h"
#include "../Views/Menu/MiniEscenarioView.h"
#include "../Views/Menu/MiniMapView.h"
#include "Mensajero.h"

static const string TAG = "ClientGameController";

ClientGameController::ClientGameController(Mensajero *mensajero) {
	this->mensajero = mensajero;

	this->shouldQuit = false;
	this->renderer = NULL;
	this->escenario = NULL;
	this->escenarioView = NULL;
	this->miniEscenarioView = NULL;
	this->config = NULL;
	this->middlePoint = 0;
	this->vertixSlope = 0;
}

ClientGameController::~ClientGameController() {}

void ClientGameController::agregarEntidades(list<Entity*> entidades) {
	bool updated = false;
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		if (entidadReal != this->escenario->getProtagonista()) {
			updated = true;
			EntityView* entityView = new EntityView(entidadReal->getNombre());
			entityView->setModel(entidadReal);
			this->escenarioView->addEntityView(entityView);


			// agrego mini vista
			string miniName = MiniView::NombreDrawableFromNombreTipo(entidadReal->getNombre());
			MiniView *miniView = new MiniView(miniName);
			miniView->setModel(entidadReal);
			this->miniEscenarioView->addEntityMiniView(miniView);
		}
	}
	if (updated){
		this->renderer->updatedEscenario();
		this->renderer->updatedMiniEscenario();
	}
}

void ClientGameController::actualizarEntidades(list<Entity*> entidades) {
	this->escenarioView->getEntitiesView()->clear();
	this->miniEscenarioView->getEntitiesMiniView()->clear();
	// Agrego vista del personaA>je
	this->initPersonaje();

	this->agregarEntidades(entidades);
}

void ClientGameController::initMap(){
	// Crear vistas a partir de la configuracion
	MapView *mapView = new MapView(TILE_DEFAULT_NAME);
	mapView->setModel(this->escenario->mundo);
	this->escenarioView = new EscenarioView(mapView);
	this->renderer->setEscenarioView(this->escenarioView);

	//creo mini escenario
	MiniMapView *miniMapView = new MiniMapView(MINI_TILE_DEFAULT_NAME);
	miniMapView->setModel(this->escenario->mundo);
	this->miniEscenarioView = new MiniEscenarioView(miniMapView);
	this->renderer->setMiniEscenarioView(this->miniEscenarioView);
}

void ClientGameController::initEntities(){
	// Agrego todas las vistas (siempre que no sean el protagonista)
	list<Entity*> entidades = escenario->getListaEntidades();
	this->agregarEntidades(entidades);
}

void ClientGameController::initPersonaje() {
	// Agrego vista del personaje
	MobileView *personajeView = new MobileView(this->escenario->getProtagonista()->getNombre());
	personajeView->setModel(this->escenario->getProtagonista());
	this->escenarioView->addEntityView(personajeView);
	this->renderer->updatedEscenario();

	// agrego mini vista del personaje
	string miniPersonajeName = MiniView::NombreDrawableFromNombreTipo(this->escenario->getProtagonista()->getNombre());
	MiniView *miniPersonajeView = new MiniView(miniPersonajeName);
	miniPersonajeView->setModel(this->escenario->getProtagonista());
	this->miniEscenarioView->addEntityMiniView(miniPersonajeView);
	this->renderer->updatedMiniEscenario();
}

float ClientGameController::scrollingSpeed(int z, int min, int max) {
	//estoy fuera del escenario
	if ((z <= min) || (z >= max)){
		return 0;
	}

	int margenScroll = this->config->getMargenScroll();
	int scrollInferior = (min + margenScroll);
	int scrollSuperior = (max - margenScroll);

	// esta en la zona de scroll inferior, devuelvo la velocidad proporcional
	if (z < scrollInferior){
		float distancia = (float) (z - scrollInferior);
		return (float)(distancia / margenScroll);
	}

	// esta en la zona de scroll superior, devuelvo la velocidad proporcional
	if (z > scrollSuperior){
		float distancia = (float) (z - scrollSuperior);
		return (float)(distancia / margenScroll);
	}

	return 0;
}

void ClientGameController::initWindowSizes() {
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

float ClientGameController::scrollingSpeedX(int x) {
	SDL_Point escenarioSize = this->renderer->escenarioSize();
	return scrollingSpeed(x,0,escenarioSize.x)*-1;
}

float ClientGameController::scrollingSpeedY(int y) {
	SDL_Point escenarioSize = this->renderer->escenarioSize();
	int minY = TOP_BAR_HEIGHT;
	int maxY = TOP_BAR_HEIGHT+escenarioSize.y;
	return scrollingSpeed(y,minY,maxY)*-1;
}

SDL_Point ClientGameController::getMaxVertixForPoint(int yPosition) {
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

void ClientGameController::moveToPoint(SDL_Point point) {
	SDL_Point escenarioSize = this->renderer->escenarioSize();
	// Checkea que el scroll no se vaya por las perpendiculares
	point.y = (point.y > intialPointWindowWrapper.y) ? intialPointWindowWrapper.y :point.y;
	point.y = (point.y < (finalPointWindowWrapper.y + escenarioSize.y)) ? (finalPointWindowWrapper.y + escenarioSize.y) : point.y;
	point.x = (point.x > intialPointWindowWrapper.x) ? intialPointWindowWrapper.x : point.x;
	point.x = (point.x < (finalPointWindowWrapper.x + (1.5*escenarioSize.x))) ? (finalPointWindowWrapper.x + (1.5*escenarioSize.x)) :point.x;

	// Checkea que el scroll no se vaya por las diagonales
	SDL_Point maxVertix = this->getMaxVertixForPoint(point.y);
	point.x = (point.x < maxVertix.x)? maxVertix.x: point.x;
	point.x = (point.x > maxVertix.y)? maxVertix.y: point.x;

	this->renderer->mainTilePosition = point;
}

void ClientGameController::updateWindow() {
	//Get mouse position
	int x, y, newY, newX;
	SDL_GetMouseState(&x, &y);

	newX = (scrollingSpeedX(x)*SCROLL_SPEED) + this->renderer->mainTilePosition.x;
	newY = (scrollingSpeedY(y)*SCROLL_SPEED) + this->renderer->mainTilePosition.y;

	if ((newX != this->renderer->mainTilePosition.x) || (newY != this->renderer->mainTilePosition.y)) {
		this->moveToPoint({newX,newY});
	}
}

bool ClientGameController::pollEvents(){
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


void ClientGameController::close() {
	if (this->renderer){
		this->renderer->close();
		delete this->renderer;
	}

	delete this->escenarioView;
	this->escenarioView = NULL;

	delete this->miniEscenarioView;
	this->miniEscenarioView = NULL;
}

// Mixtos
bool ClientGameController::play() {
	while( !this->mensajero->inicializado()) {
		return false;
	}
	this->config = this->mensajero->obtenerConfiguracion();
	this->escenario = this->mensajero->obtenerEscenario();

	this->renderer = new Renderer(this->config->getPantallaAncho(),this->config->getPantallaAlto(), this->config->getTipos());
		if (!this->renderer->canDraw()){
			Log().Get(TAG,logERROR) << "Failed to initialize Renderer!";
			this->close();
			return false;
		}

	this->initMap();
	this->initEntities();
	this->initPersonaje();
	this->initWindowSizes();

	bool shouldRestart = false;
	//While application is running
	while( !this->shouldQuit && !shouldRestart ) {
		this->updateWindow();
		shouldRestart = this->pollEvents();
		this->renderer->drawViews();
		this->sleep();
	}

	this->close();
	return shouldRestart;
}

void ClientGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}
