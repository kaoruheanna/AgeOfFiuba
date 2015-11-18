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
#include <cmath>
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

	this->pendingEntity = NULL;
	this->futureBuildingView = NULL;
	this->empezoPartida = false;
	this->mouseDown = false;
	this->posInicialMouse = {0,0};

}

ClientGameController::~ClientGameController() {
	this->selectedEntities.clear();
}

void ClientGameController::comenzoPartida() {
	this->empezoPartida = true;
	if(this->renderer != NULL){
		this->renderer->hideCartel();
	}
}

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
	//TODO ojo revisar los corchetes
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

		if(this->pendingEntity != NULL){
			int x, y;
			SDL_GetMouseState(&x, &y);
			SDL_Point posicion = this->renderer->windowToMapPoint({x,y});
			this->pendingEntity->setPosicion(posicion);

			LogicPosition logicPosition = LogicPosition(posicion.x,posicion.y);

			bool puedoConstruir = this->escenario->puedeConstruirEntidad(this->pendingEntity,posicion);
			FuturePositionType positionType = (puedoConstruir) ? FuturePositionTypeAllowed : FuturePositionTypeForbidden;

			if (this->futureBuildingView){
				this->futureBuildingView->setLogicPosition(logicPosition);
				this->futureBuildingView->setPositionType(positionType);
			} else {
				this->futureBuildingView = new FutureBuildingView(this->pendingEntity->getNombre(),positionType,logicPosition);
			}
		}

		if (e.type == SDL_MOUSEBUTTONDOWN && !this->serverError){
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			printf("mouse: %i, %i \n", x,y);
			this->posInicialMouse = {x,y};
			bool leftClick = (e.button.button == SDL_BUTTON_LEFT);
			if (leftClick){this->mouseDown = true;};
			this->renderer->clickEvent(x,y,leftClick,this);
		}

		if (e.type == SDL_MOUSEBUTTONUP && !this->serverError){
			if(e.button.button == SDL_BUTTON_LEFT){
				int x,y;
				SDL_GetMouseState(&x,&y);
				this->renderer->leftMouseUpEvent(this, this->posInicialMouse.x, this->posInicialMouse.y);
				this->mouseDown = false;
			}
		}

		if( e.type == SDL_MOUSEMOTION && !this->serverError){
			bool leftClick = (e.button.button == SDL_BUTTON_LEFT);
			if (this->mouseDown){
				int x,y;
				SDL_GetMouseState(&x,&y);
				this->renderer->dragLeftClickEvent(this->posInicialMouse.x,this->posInicialMouse.y,x,y);

				if(this->pendingEntity != NULL){
					if(leftClick && this->renderer->isPixelInEscenario(x,y)){
						//construyo
						this->mensajero->construir(this->pendingEntity);
						this->limpiarConstruccion();
						return pressedR;
					}
					Log().Get(TAG) << "Cancelo la construccion";
					this->limpiarConstruccion();
				}
				this->renderer->clickEvent(x,y,leftClick,this);
			}
		}
	}
	return pressedR;
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

	if(!this->empezoPartida){
		this->renderer->setCartel("Esperando usuarios...");
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

		this->renderer->setFutureBuildingView(this->futureBuildingView);
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

void ClientGameController::close() {
	if (this->renderer){
		this->renderer->close();
		delete this->renderer;
	}

	delete this->escenarioView;
	this->escenarioView = NULL;

	delete this->miniEscenarioView;
	this->miniEscenarioView = NULL;

	this->selectedEntities.clear(); //TODO ver si clear borra las entidades.

	delete this->pendingEntity;
	this->pendingEntity = NULL;
}

bool ClientGameController::isAlive() {
	return !this->shouldQuit;
}

void ClientGameController::sleep(){
	SDL_Delay(DELAY_MILISEC);
}

// el server le avisa que se actualizaron personajes
void ClientGameController::actualizaPersonaje(MobileModel* entity) {
	if (!this->inicializado())
		return;

	Entity* model = this->escenario->entidadConId(entity->getId());
	if(model == NULL){
//		Log().Get(TAG) << "No existia el personaje, tiene que crearse";
		// no existia, tiene q crearse
//		Log().Get(TAG) << "Creo entity desde actualizaPersonaje";
		MobileModel* newModel = new MobileModel(*entity);
		this->escenario->agregarEntidad(newModel);
		this->escenario->actualizarTileOcupadaPorPersonaje(newModel);
		// TODO cambiar como detecta este numero

		this->updated = true;
		return;
	}

	if(model->getClass() != MOBILE_MODEL){
		// El modelo no se mueve y.y
		return;
	}
	MobileModel* protagonista = (MobileModel*)model;
	protagonista->update(entity);
	this->escenario->actualizarTileOcupadaPorPersonaje(protagonista);
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
	if (!this->inicializado()) {
		return;
	}

	Log().Get(TAG) << entity->getNombre() <<"me llego con tamaño:"<<entity->getAnchoBase()<<"x"<<entity->getAltoBase();

	Entity* existingEntity = this->escenario->entidadConId(entity->getId());

	if(existingEntity) {
		if(existingEntity->getClass() == MOBILE_MODEL) {
			delete entity;
			return;
		}

		existingEntity->update(entity);
		if (!existingEntity->estaViva()) {
			this->escenario->eliminarEntidadConID(existingEntity->getId());
		}
		delete entity;
	} else {
		//Hecho asi creo q la entity esta viva del lado del server
		SDL_Point posicion = entity->getPosicion();
		this->escenario->construirEntidad(entity,posicion);
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
	SDL_Point tile = this->renderer->windowToMapPoint({x,y});
	Entity* entidad = this->escenario->getEntidadEnPosicion(tile);
	if (this->selectedEntities.size() == 1 and this->selectedEntities.front() == entidad){
		return;
	}
	std::pair<SDL_Point,SDL_Point> tiles;
	list<pair<SDL_Point,SDL_Point>> listaDeTile;
	if (entidad!=NULL){
		this->selectedEntities.clear();
		this->selectedEntities.push_front(entidad);
		this->setMessageForSelectedEntity(entidad);
		tiles = this->escenario->getTilesCoordinatesForEntity(entidad);
		listaDeTile.push_front(tiles);
		entidad->creables = this->getCreablesListForEntityName(entidad->getNombre());
		this->renderer->setSelectedTilesCoordinates(true,listaDeTile,this->selectedEntities);

	}else{
		this->selectedEntities.clear();
		this->renderer->setMessagesInMenu("Selecciona algo!!", "");
		list<Entity*> listaVacia;
		this->renderer->setSelectedTilesCoordinates(false,listaDeTile,listaVacia);
	}
}

list<string> ClientGameController::getCreablesListForEntityName(string name){
	list<string> creables;
	creables.clear();
	list<TipoConfig> typesList = this->config->getTipos();
	list<TipoConfig>::iterator elementoDeTipoActual;
	TipoConfig aux;
	for (elementoDeTipoActual = typesList.begin(); elementoDeTipoActual != typesList.end(); ++elementoDeTipoActual){
		aux = *elementoDeTipoActual;
		if (aux.getNombre() == name){
			creables = aux.getCreables();
			return creables;
		}
	}
	return creables;
}

void ClientGameController::rightClickEnEscenario(int x, int y) {
	if(this->selectedEntities.empty()){
		// No se le puede dar ordenes a la nada
		return;
	}
	SDL_Point point = this->renderer->windowToMapPoint({x,y});
	Entity *entidad = this->escenario->getEntidadEnPosicion(point);

	if(entidad) {
		for (Entity* selectedEntity: this->selectedEntities){
			if (selectedEntity->getId() != entidad->getId()) {
				this->mensajero->interactuar(selectedEntity->getId(),entidad->getId());
			}
		}
		return;
	}
	// Mover el personaje seleccionado a la nueva posicion
	point = this->renderer->proyectedPoint(point, this->escenario->getSize());


	//TODO mover unidades
	this->moverMuchasUnidades(point);


}

void ClientGameController::leftMouseUp(int x, int y, int w, int h){
	list<Entity*> listaDeEntidadesSeleccionadas;
	SDL_Point mapPointInicial = this->renderer->windowToMapPoint({x,y});
	SDL_Point mapPointFinal = this->renderer->windowToMapPoint({x+w,y+h});
	//TODO getEntidadesEnAreaForJugador no anda 100% bien.
	listaDeEntidadesSeleccionadas = this->escenario->getEntidadesEnAreaForJugador(mapPointInicial, mapPointFinal,this->usuario->getTeam());
	this->setSelectedEntities(listaDeEntidadesSeleccionadas);
	list <pair<SDL_Point,SDL_Point>> tiles = this->escenario->getTilesCoordinatesForEntities(this->selectedEntities);
	if (!this->selectedEntities.empty()){
		this->setMessageForSelectedEntities(listaDeEntidadesSeleccionadas);
		tiles = this->escenario->getTilesCoordinatesForEntities(listaDeEntidadesSeleccionadas);
		//TODO ver como devolver los creables para un conjunto de unidades.
		this->setCreablesForEntities(this->selectedEntities);
		//entidad->creables = this->getCreablesListForEntityName(entidad->getNombre());
		this->renderer->setSelectedTilesCoordinates(true,tiles,listaDeEntidadesSeleccionadas);
	}else{
		this->renderer->setMessagesInMenu("Selecciona algo!!", "");
		this->renderer->setSelectedTilesCoordinates(false,tiles,listaDeEntidadesSeleccionadas);
	}
}

void ClientGameController::moverMuchasUnidades(SDL_Point destino){
	queue<SDL_Point> tiles = this->obtenerTilesParaMoverse(destino);
	for (Entity* entidad: this->selectedEntities){
		//TODO toda entidad deberia tener un destino asignado.
		if (!tiles.empty()){
			std::cout<<entidad->getNombre()<<"\n";
			this->moverUnaUnidad(entidad, tiles.front());
			tiles.pop();
		}
	}
}

void ClientGameController::moverUnaUnidad(Entity* entidad, SDL_Point destino){
	MobileModel* auxModel = new MobileModel();
	auxModel->setId(entidad->getId());
	auxModel->setDestination(destino.x, destino.y);
	this->mensajero->moverEntidad(auxModel, username);
	delete auxModel;
}

float getAnguloForDireccion(SDL_Point direccion){
	float angulo;
	if (direccion.x == 0){
		angulo = 90;
	}else{
		angulo = (atan(direccion.y/direccion.x)*180)/M_PI;
	}
	if (direccion.x < 0){
		angulo += 180;
	}else if(direccion.y < 0){
		angulo += 360;
	}
	return angulo;
}

SDL_Point obtenerDireccionPerpendicularParaAngulo(float angulo){
	SDL_Point dirFormacion = {0,0};
	if ((angulo >= 0 and angulo <= 22.5) or (angulo > 337.5)){
		dirFormacion = {0,TILE_SIZE};
	}else if (angulo > 22.5 and  angulo <= 67.5){
		dirFormacion = {-TILE_SIZE,TILE_SIZE};
	}else if (angulo > 67.5 and angulo <= 112.5){
		dirFormacion = {-TILE_SIZE,0};
	}else if (angulo > 112.5 and angulo <= 157.5){
		dirFormacion = {-TILE_SIZE,-TILE_SIZE};
	}else if (angulo > 157.5 and angulo <= 202.5){
		dirFormacion = {0,-TILE_SIZE};
	}else if (angulo > 202.5 and angulo <= 247.5){
		dirFormacion = {TILE_SIZE,-TILE_SIZE};
	}else if (angulo > 247.5 and angulo <= 292.5){
		dirFormacion = {TILE_SIZE,0};
	}else if (angulo > 292.5 and angulo <= 337.5){
		dirFormacion = {TILE_SIZE,TILE_SIZE};
	}
	return dirFormacion;
}

queue<SDL_Point> ClientGameController::obtenerTilesParaMoverse(SDL_Point destino){
	queue<SDL_Point> listaDeTiles;
	int x = destino.x - (destino.x%TILE_WIDTH_PIXELS) + (TILE_WIDTH_PIXELS/2);
	int y = destino.y - (destino.y%TILE_HEIGHT_PIXELS) + (TILE_HEIGHT_PIXELS/2);
	SDL_Point destinoCentrado = {x,y};

	SDL_Point posMedia = this->getPosicionPromedioForSelectedEntities();
	int cantidadDeTiles = this->selectedEntities.size();
	int tamFila = 5;
	SDL_Point direccion = {destinoCentrado.x - posMedia.x, destinoCentrado.y - posMedia.y};
	float angulo = getAnguloForDireccion(direccion);
	SDL_Point dirFormacion = obtenerDireccionPerpendicularParaAngulo(angulo);
	printf("angulo: %f, direccion: %i, %i \n",angulo, dirFormacion.x, dirFormacion.y);
	int filas = cantidadDeTiles/tamFila;
	int i = 0;
	while (i < cantidadDeTiles){//TODO mejorar para no tener mas de una fila
		SDL_Point tile = {destinoCentrado.x + (dirFormacion.x*i), destinoCentrado.y + (dirFormacion.y*i)};
		//TODO ver si el tile es valido.
		if (this->escenario->posicionValidaParaCaminar(tile)){
			listaDeTiles.push(tile);
		}
		i+=1;
	}

	return listaDeTiles;
}



SDL_Point ClientGameController::getPosicionPromedioForSelectedEntities(){
	SDL_Point sumaDePosiciones = {0,0};
	for (Entity* entidad: this->selectedEntities){
		sumaDePosiciones.x += entidad->getPosicion().x;
		sumaDePosiciones.y += entidad->getPosicion().y;
	}
	sumaDePosiciones.x /= this->selectedEntities.size();
	sumaDePosiciones.y /= this->selectedEntities.size();
	return sumaDePosiciones;
}

void ClientGameController::setSelectedEntities(list<Entity*> listaDeEntidades){
	this->selectedEntities.swap(listaDeEntidades);
}

void ClientGameController::setCreablesForEntities(list<Entity*> listaDeEntidades){
	for (Entity* entidad: listaDeEntidades){
		entidad->creables = this->getCreablesListForEntityName(entidad->getNombre());
	}
}

void ClientGameController::setMessageForSelectedEntity(Entity* entity){
	if (entity == NULL){return;}
	string equipo = "";
	switch(entity->getTeam()){
		case TEAM_RED:
			equipo = NOMBRE_EQUIPO_RED;
			break;
		case TEAM_BLUE:
			equipo = NOMBRE_EQUIPO_BLUE;
			break;
		case TEAM_GREEN:
			equipo = NOMBRE_EQUIPO_GREEN;
			break;
		case TEAM_YELLOW:
			equipo = NOMBRE_EQUIPO_YELLOW;
			break;
		default:
			equipo = NOMBRE_EQUIPO_NEUTRAL;

	}
	if (!(entity->esJugador())){
		this->renderer->setMessagesInMenu("Entidad - "+ equipo,entity->getNombreAMostrar());
		return;
	}

	this->renderer->setMessagesInMenu("Jugador - "+ equipo,entity->getNombreAMostrar());
}

/*selecciona un mensaje para una lista de entidades
 * Por ahora solo toma el mensaje para la primera unidad
 * TODO inteligencia para seleccionar mensaje.
 */
void ClientGameController::setMessageForSelectedEntities(list<Entity*> entities){
	Entity* entity = entities.front();
	this->setMessageForSelectedEntity(entity);
}

void ClientGameController::createEntityButtonPressed(string entityName) {
	this->limpiarConstruccion();

	if (this->escenario->factory->esBuilding(entityName)){
		this->pendingEntity = this->escenario->factory->crearEntidadParaConstruir(entityName,this->selectedEntities.front()->getPosicion(),this->selectedEntities.front()->getTeamString());
		return;
	}

	list<TileCoordinate> tiles = this->escenario->getVecinosLibresForEntity(this->selectedEntities.front());
	if (tiles.size() == 0){
		Log().Get(TAG) << "No hay espacio para crear la entidad";
		return;
	}

	TileCoordinate tile = tiles.front();
	SDL_Point tilePoint = {tile.first,tile.second};

	Entity *selectedEntity = this->selectedEntities.front();

	std::pair<SDL_Point,SDL_Point> tilesEntity = this->escenario->getTilesCoordinatesForEntity(selectedEntity);
	int minX = tilesEntity.first.x;
	int minY = tilesEntity.first.y;
	int maxX = tilesEntity.second.x;
	int maxY = tilesEntity.second.y;
//	Log().Get(TAG) << "la entidad: "<<selectedEntity->getNombre() <<" ocupa los tiles:("<<minX<<","<<minY<<") al ("<<maxX<<","<<maxY<<")";
//	Log().Get(TAG) << selectedEntity->getAnchoBase() <<"x"<<selectedEntity->getAltoBase();


	Log().Get(TAG) << "y crea una unidad en ("<<tilePoint.x<<","<<tilePoint.y<<")";


	Entity *tempEntity = this->escenario->factory->crearEntidadParaConstruir(entityName,tilePoint,selectedEntity->getTeamString());

	this->mensajero->construir(tempEntity);
	delete tempEntity;
}

//libera las cosas que usa para la construccion
void ClientGameController::limpiarConstruccion(){
	if (this->pendingEntity){
		delete this->pendingEntity;
		this->pendingEntity = NULL;
	}

	if (this->futureBuildingView){
		this->futureBuildingView = NULL;
		delete this->futureBuildingView;
	}
	this->renderer->setFutureBuildingView(NULL);
}

void ClientGameController::actualizarRecursos(User* auxUser){
	this->usuario->update(auxUser);
}
