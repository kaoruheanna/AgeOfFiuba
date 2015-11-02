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
	this->usuario = NULL;
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
	this->selectedEntity = NULL;
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
	// Agrego todas las entidades
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		switch(entidadReal->getClass()){
			case MOBILE_MODEL:
				this->agregarPersonaje((MobileModel*)entidadReal);
				break;
			default:
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
			bool leftClick = (e.button.button == SDL_BUTTON_LEFT);
			this->renderer->clickEvent(x,y,leftClick,this);
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

bool ClientGameController::play() {
	this->mensajero->addClient(this);

	while (!this->inicializado()) {
		this->sleep();
	}

	while(usuario == NULL){
		this->sleep();
	}

	this->renderer = new Renderer(this->config->pantalla.getAncho(),this->config->pantalla.getAlto(), this->escenario->tiposConfigList);
	this->renderer->setProtagonista(usuario);

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

	Team currentTeam = this->usuario->getTeam();

	//While application is running
	while( !this->shouldQuit && !shouldRestart && !this->serverError ) {
		if(this->updated) {
			actualizarEntidades(this->escenario->getListaEntidades());
			this->updated = false;
		}

		this->updateWindow();
		shouldRestart = this->pollEvents();

		//positionCharacter =(this->escenario->getProtagonista())->getPosicion();
		//positionCharacter = this->escenario->mundo->getTileForPosition(positionCharacter);
		this->renderer->fogUpdate(this->escenario->getListaEntidades(),currentTeam);

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

	Entity* model = this->escenario->entidadConId(entity->getId());
	if(model == NULL){
		this->escenario->agregarEntidad(entity);
		// TODO cambiar como detecta este numero
		posicionInicialProtagonista = entity->getPosicion();
		return;
	}
	if(model->getClass() != MOBILE_MODEL){
		// El modelo no se mueve y.y
		return;
	}
	MobileModel* protagonista = (MobileModel*)model;
	protagonista->update(entity);
}


void ClientGameController::cambioUsuario(User* user) {
	// Si el usuario es el de este cliente actualizar su data
	printf("usuario: %s current: %s\n", user->getName().c_str(), username.c_str());
	if(user->getName().compare(username) == 0){
		if(this->usuario == NULL){
			// Si no tiene usuario crearlo
			User* usuario = new User(username);
			usuario->update(user);
			this->usuario = usuario;
		} else {
			this->usuario->update(user);
		}
	}
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

	if(this->escenario->eliminarEntidadConID(recurso->getId())) {
		this->updated = true;
	}
}

void ClientGameController::actualizarEntidad(Entity* entity) {
	if (!this->inicializado())
			return;

	Entity* newEntity = this->escenario->entidadConId(entity->getId());

	if( newEntity->getClass() == MOBILE_MODEL) {
		return;
	}

	if(newEntity) {
		newEntity->update(entity);
		if (!newEntity->estaViva()) {
			this->escenario->eliminarEntidadConID(newEntity->getId());
		}
	} else {
		this->escenario->agregarEntidad(entity);
	}

	this->updated = true;
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

/*
 * RendererInteractionDelegate
 */
void ClientGameController::leftClickEnEscenario(int x,int y){
	SDL_Point point = this->renderer->windowToMapPoint({x,y});
	Entity *entidad = this->escenario->getEntidadEnPosicion(point);
	if (this->selectedEntity == entidad)
		return;

	this->selectedEntity = entidad;

	std::pair<SDL_Point,SDL_Point> tiles;
	if(this->selectedEntity != NULL){
		this->setMessageForSelectedEntity(entidad);
		tiles = this->escenario->getTilesCoordinatesForEntity(entidad);
		this->renderer->setSelectedTilesCoordinates(true,tiles,entidad);
	} else {
		this->renderer->setMessagesInMenu("Selecciona algo!!", "");
		this->renderer->setSelectedTilesCoordinates(false,tiles,NULL);
	}
}

void ClientGameController::rightClickEnEscenario(int x, int y) {
	if(this->selectedEntity == NULL){
		// No se le puede dar ordenes a la nada
		return;
	}
	SDL_Point point = this->renderer->windowToMapPoint({x,y});
	Entity *entidad = this->escenario->getEntidadEnPosicion(point);

	if(entidad && (this->selectedEntity->getId() != entidad->getId())) {
		//Interactuar la seleccionada con la nueva entidad
		this->mensajero->interactuar(this->selectedEntity->getId(),entidad->getId());
		return;
	}
	// Mover el personaje seleccionado a la nueva posicion
	point = this->renderer->proyectedPoint(point, this->escenario->getSize());

	MobileModel* auxModel = new MobileModel();
	auxModel->setId(this->selectedEntity->getId());
	auxModel->setDestination(point.x, point.y);
	this->mensajero->moverEntidad(auxModel, username);
	delete auxModel;
}

void ClientGameController::setMessageForSelectedEntity(Entity* entity){
	string equipo = "";
	switch(entity->getTeam()){
		case RED:
			equipo = "RED";
			break;
		case BLUE:
			equipo = "BLUE";
			break;
		case GREEN:
			equipo = "GREEN";
			break;
		case YELLOW:
			equipo = "YELLOW";
			break;
		default:
			equipo = "NEUTRAL";

	}
	if (!(entity->esJugador())){
		this->renderer->setMessagesInMenu("Entidad - "+ equipo,entity->getNombreAMostrar());
		return;
	}

	this->renderer->setMessagesInMenu("Jugador - "+ equipo,entity->getNombreAMostrar());

}

void ClientGameController::createEntityButtonPressed(string entityName) {
	Log().Get(TAG) << "Create entity button pressed: " << entityName;
}
