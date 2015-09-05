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
#include <unistd.h>

GameController::GameController() {
	this->shouldQuit = false;
	this->renderer = NULL;
	this->model = NULL;
}

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}

void GameController::play() {
	this->renderer = new Renderer();
	if (!this->renderer->canDraw()){
		printf( "Failed to initialize!\n" );
		this->close();
		return;
	}

	this->model = new MobileModel();
	this->model->setX(200);
	this->model->setY(100);

	MobileView *marioView = new MobileView();
	marioView->setModel(model);
	this->views.push_back(marioView);
	this->renderer->addView(marioView);

	//While application is running
	while( !this->shouldQuit ) {
		this->pollEvents();
		this->model->updatePosition();
		this->renderer->draw();
		this->sleep();
	}

	this->close();
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
			SDL_GetMouseState( &x, &y );
			this->model->setDestination(x,y);
//			this->model->updatePosition();
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
	int microsec = millisec * 1000;
	usleep(microsec);
}
