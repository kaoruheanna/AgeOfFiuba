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

const std::string TAG = "GameController";

GameController::GameController(GameConfiguration *config) {
	this->shouldQuit = false;
	this->renderer = NULL;
	this->model = NULL;
	this->config = config;
}

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}

void GameController::play() {
	Log().Get(TAG,logDEBUG) << "[GameController] " << "play";

	this->renderer = new Renderer(this->config->getPantallaAncho(),this->config->getPantallaAlto(), this->config->getTipos());
	if (!this->renderer->canDraw()){
		printf( "Failed to initialize!\n" );
		this->close();
		return;
	}

	//Agrego el mapa
	Map* mapModel = new Map(100,100,128,64);
	MapView *mapView = new MapView("tileDefault");
	mapView->setModel(mapModel);
	this->views.push_back(mapView);
	this->renderer->addView(mapView);

	//Agrego una casa
	SDL_Point pos1 = {4,3};
	Entity* casa1 = new Entity("casa",pos1,2,2);
	EntityView* casa1View = new EntityView("casa");
	casa1View -> setModel(casa1);
	this->views.push_back(casa1View);
	this->renderer->addView(casa1View);

	//Agrego otra casa
	SDL_Point pos2 = {10,4};
	Entity* casa2 = new Entity("casa",pos2,2,2);
	EntityView* casa2View = new EntityView("casa");
	casa2View -> setModel(casa2);
	this->views.push_back(casa2View);
	this->renderer->addView(casa2View);

	//Agrego otra casa
	SDL_Point pos3 = {1,1};
	Entity* casa3 = new Entity("casa",pos3,2,2);
	EntityView* casa3View = new EntityView("casa");
	casa3View -> setModel(casa3);
	this->views.push_back(casa3View);
	this->renderer->addView(casa3View);

	//Agrego otra casa
	SDL_Point pos4 = {3,7};
	Entity* casa4 = new Entity("casa",pos4,2,2);
	EntityView* casa4View = new EntityView("casa");
	casa4View -> setModel(casa4);
	this->views.push_back(casa4View);
	this->renderer->addView(casa4View);


	this->model = new MobileModel();
	this->model->setX(200);
	this->model->setY(100);

	MobileView *marioView = new MobileView("soldado");
	marioView->setModel(model);
	this->views.push_back(marioView);
	this->renderer->addView(marioView);


	//While application is running
	while( !this->shouldQuit ) {
		this->updateWindow();
		this->pollEvents();
		this->model->updatePosition();
		this->renderer->drawViews();
		this->sleep();
	}

	this->close();
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

void GameController::pollEvents(){
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) {

		if( e.type == SDL_QUIT ) {
			printf("tengo que cerrarlo \n");
			this->shouldQuit = true;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN){
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			SDL_Point mapPoint = this->renderer->windowToMapPoint({x,y});
			this->model->setDestination(mapPoint.x,mapPoint.y);
		}
	}
}

void GameController::close() {
	if (this->renderer){
		this->renderer->close();
		delete this->renderer;
	}

	if (this->model){
		delete this->model;
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
	int millisec = 20;
	SDL_Delay(millisec);
}
