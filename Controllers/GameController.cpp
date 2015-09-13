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
#include "../Views/MapView.h"
#include "../Utils/Log.h"
#include "../Configuration/GameConfiguration.h"

GameController::GameController() {
	this->shouldQuit = false;
	this->renderer = NULL;
	this->model = NULL;
}

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}

void GameController::play() {
	Log().Get(logDEBUG) << "[GameController] " << "play";

	this->renderer = new Renderer();
	if (!this->renderer->canDraw()){
		printf( "Failed to initialize!\n" );
		this->close();
		return;
	}

	this->model = new MobileModel();
	this->model->setX(200);
	this->model->setY(100);

	MobileView *marioView = new MobileView("soldier");
	marioView->setModel(model);
	this->views.push_back(marioView);
	this->renderer->addView(marioView);

	//Agrego el mapa
	Map* mapModel = new Map(100,100,128,64);

	MapView *mapView = new MapView("tileDefault");
	mapView->setModel(mapModel);
	this->views.push_back(mapView);
	this->renderer->addView(mapView);


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

const int SCROLL_SPEED = 2;
const int SCROLL_WINDOW_WIDTH = 50;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


float GameController::scrollingSpeed(int x, int large) {
	if ((x < SCROLL_WINDOW_WIDTH) && (x>0)) {
		return ((float)(x-SCROLL_WINDOW_WIDTH)/SCROLL_WINDOW_WIDTH);
	}
	if ((x > (large - SCROLL_WINDOW_WIDTH)) && (x < large)) {
		return ((float)(x - large + SCROLL_WINDOW_WIDTH)/SCROLL_WINDOW_WIDTH);
	}
	return 0;
}

float GameController::scrollingSpeedX(int x) {
	return scrollingSpeed(x,SCREEN_WIDTH);
}

float GameController::scrollingSpeedY(int y) {
	return scrollingSpeed(y,SCREEN_HEIGHT);
}

void GameController::moveToPoint(SDL_Point point) {
	this->renderer->mainTilePosition = point;
//	Log().Get(logDEBUG) << "NewPoint: " << this->renderer->mainTilePosition.x << "," << this->renderer->mainTilePosition.y;
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
