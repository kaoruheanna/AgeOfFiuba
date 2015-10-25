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
	this->config = NULL;

	this->shouldQuit = false;
	this->renderer = NULL;
	this->escenario = NULL;
	this->escenarioView = NULL;
	this->miniEscenarioView = NULL;

	this->middlePoint = 0;
	this->vertixSlope = 0;

	this->updated = false;
	this->serverError = false;
}

ClientGameController::~ClientGameController() {}

void ClientGameController::agregarEntidad(Entity* entidad) {
	EntityView* entityView = new EntityView(entidad->getNombre());
	entityView->setModel(entidad);
	this->escenarioView->addEntityView(entityView);


	// agrego mini vista
	string miniName = MiniView::NombreDrawableFromNombreTipo(entidad->getNombre());
	MiniView *miniView = new MiniView(miniName);
	miniView->setModel(entidad);
	this->miniEscenarioView->addEntityMiniView(miniView);
}

void ClientGameController::agregarPersonaje(MobileModel* personaje) {
	bool esProtagonista = (personaje->getUsername().compare(this->username) == 0);
	if(esProtagonista){
		this->renderer->setProtagonista(this->escenario->getProtagonista());
	}

	// Agrego vista del personaje
	MobileView *personajeView = new MobileView(personaje->getNombre());
	personajeView->setModel(personaje);
	this->escenarioView->addEntityView(personajeView);

	// Agrego mini vista del personaje
	string miniPersonajeName = MiniView::NombreDrawableFromNombreTipo(personaje->getNombre());
	MiniView *miniPersonajeView = new MiniView(miniPersonajeName);
	miniPersonajeView->setModel(personaje);
	this->miniEscenarioView->addEntityMiniView(miniPersonajeView);
}

void ClientGameController::actualizarEntidades(list<Entity*> entidades) {
	this->escenarioView->getEntitiesView()->clear();
	this->miniEscenarioView->getEntitiesMiniView()->clear();
	// Agrego todos los personajes
	map<string, MobileModel*>::iterator found;
	for(found = this->escenario->usuarios.begin(); found != this->escenario->usuarios.end(); ++found){
		this->agregarPersonaje(found->second);
	}
	// Agrego el resto de las entidades
	string nombre = this->escenario->getProtagonista()->getNombre();
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		if (nombre.compare(entidadReal->getNombre()) != 0) {
			this->agregarEntidad(entidadReal);
		}
	}
	// Refrescar las vistas
	this->renderer->updatedEscenario();
	this->renderer->updatedMiniEscenario();
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

float ClientGameController::scrollingSpeed(int z, int min, int max) {
	//estoy fuera del escenario
	if ((z <= min) || (z >= max)){
		return 0;
	}

	int margenScroll = this->config->configuracion.getScroll();
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

	intialPointWindowWrapper.x = -intialWindowWrapperLeft.x + (this->config->pantalla.getAncho()/2);
	intialPointWindowWrapper.y = -intialWindowWrapperTop.y;

	finalPointWindowWrapper.x = -intialWindowWrapperRight.x;
	finalPointWindowWrapper.y = -intialWindowWrapperBottom.y;


	Log().Get(TAG) << "Pos inicial " << this->renderer->mainTilePosition.x << "," << this->renderer->mainTilePosition.y;

	SDL_Point posicionInicial = this->renderer->mapToWindowPoint(posicionInicialProtagonista);
	this->moveToPoint({	-posicionInicial.x + this->config->pantalla.getAncho(),
						-posicionInicial.y + (this->config->pantalla.getAlto()/2)});
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
		maxVertix.x = vertixSlope * invertedYPosition + this->config->pantalla.getAlto();
		maxVertix.y = -1 * vertixSlope * invertedYPosition;
	} else {
		maxVertix.x = vertixSlope * yPosition;
		maxVertix.y = this->config->pantalla.getAncho() - vertixSlope * yPosition;
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

		if (e.type == SDL_MOUSEBUTTONDOWN && !this->serverError){
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			SDL_Point point = this->renderer->windowToMapPoint({x,y});
			Entity *entidad = this->escenario->getEntidadEnPosicion(point);

			if(e.button.button == SDL_BUTTON_RIGHT) {
				if(entidad && (selectedEntity!=entidad)) {
					//Interactuar la seleccionada con la nueva entidad

				} else {
					// Mover el personaje seleccionado a la nueva posicion
					// TODO Eliminar harcodeo de protagonista
					point = this->renderer->proyectedPoint(point, this->escenario->getSize());
					MobileModel* auxModel = new MobileModel();
					auxModel->setUsername(username);
					auxModel->setDestination(point.x, point.y);
					this->mensajero->moverProtagonista(auxModel);
					delete auxModel;
				}
			} else {
				selectedEntity = entidad;
				std::pair<SDL_Point,SDL_Point> tiles;
				if (entidad && (entidad != this->escenario->getProtagonista())){
					this->setMessageForSelectedEntity(entidad);
					tiles = this->escenario->getTilesCoordinatesForEntity(entidad);
					this->renderer->setSelectedTilesCoordinates(true,tiles);
				} else {
					this->setMessageForSelectedEntity(this->escenario->getProtagonista());
					this->renderer->setSelectedTilesCoordinates(false,tiles);
				}
			}
		}
	}
	return pressedR;
}

void ClientGameController::setMessageForSelectedEntity(Entity* entity){
	if (!(entity->esJugador())){
		this->renderer->setMessagesInMenu("Entidad",entity->getNombreAMostrar());
		return;
	}

	if (entity == this->escenario->getProtagonista()){
		this->renderer->setMessagesInMenu("Protagonista",entity->getNombreAMostrar());
		return;
	}

	this->renderer->setMessagesInMenu("Jugador",entity->getNombreAMostrar());

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

bool ClientGameController::play() {
	this->mensajero->addClient(this);

	while (!this->inicializado()) {
		this->sleep();
	}

	MobileModel* protagonista = this->escenario->getUserModel(this->username);
	while(protagonista == NULL){
		this->sleep();
		protagonista = this->escenario->getUserModel(this->username);
	}

	this->escenario->setProtagonista(protagonista);

	this->renderer = new Renderer(this->config->pantalla.getAncho(),this->config->pantalla.getAlto(), this->escenario->tiposConfigList);
		if (!this->renderer->canDraw()){
			Log().Get(TAG,logERROR) << "Failed to initialize Renderer!";
			this->close();
			return false;
		}



	this->initMap();
	this->updated = true;
	this->initWindowSizes();

	this->renderer->setFog(this->escenario->mundo->getWidth(),this->escenario->mundo->getHeight());
	SDL_Point positionCharacter = {0,0};

	bool shouldRestart = false;

	//While application is running
	while( !this->shouldQuit && !shouldRestart && !this->serverError ) {
		if(this->updated) {
			actualizarEntidades(this->escenario->getListaEntidades());
			this->updated = false;
		}

		this->updateWindow();
		shouldRestart = this->pollEvents();

		positionCharacter =(this->escenario->getProtagonista())->getPosicion();
		positionCharacter = this->escenario->mundo->getTileForPosition(positionCharacter);
		this->renderer->fogUpdate(positionCharacter.x,positionCharacter.y);

		this->renderer->drawViews();
		this->sleep();
		this->mensajero->ping();
	}

	while( !this->shouldQuit && !shouldRestart ){
		shouldRestart = this->pollEvents();
		this->renderer->drawViews();
		this->sleep();
	}

	this->close();
	return shouldRestart;
}

bool ClientGameController::isAlive() {
	return !this->shouldQuit;
}

void ClientGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}

void ClientGameController::actualizaPersonaje(MobileModel* entity) {
	if (!this->inicializado())
		return;

	MobileModel* protagonista = this->escenario->getUserModel(entity->getUsername());
	if(protagonista == NULL){
		this->escenario->addUser((char*)entity->getUsername().c_str(), entity->getPosicion());
		protagonista = this->escenario->getUserModel(entity->getUsername());
		posicionInicialProtagonista = protagonista->getPosicion();
	}
	protagonista->update(entity);
}

void ClientGameController::apareceRecurso(Resource* recurso) {
	if (!this->inicializado())
			return;

	if(!this->escenario->existeRecursoConID(recurso->getId())) {
		this->escenario->agregarEntidad(new Resource(*recurso));
	}
	this->updated = true;
}

void ClientGameController::desapareceRecurso(Resource* recurso){
	if (!this->inicializado())
		return;

	if(this->escenario->eliminarRecursoConID(recurso->getId())) {
		this->updated = true;
	}
}

void ClientGameController::configEscenario(const string path) {
	this->config = new GameConfiguration(path);
	this->escenario = new Escenario(this->config->getEscenario(), this->config->getTipos());
}

void ClientGameController::errorDeLogueo() {
	if(this->renderer != NULL){
		this->renderer->setCartel("Hay un error en la conexion.");
		this->serverError = true;
	} else {
		printf("El nombre escrito ya esta tomado. Por favor elija otro.");
		this->shouldQuit = true;
	}
}

bool ClientGameController::inicializado() {
	return escenario && escenario->inicializacionCorrecta;
}
