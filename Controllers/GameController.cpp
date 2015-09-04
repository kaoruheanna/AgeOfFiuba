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
#include "../Views/Renderer.h"
#include "../Views/View.h"

GameController::GameController() {
	this->shouldQuit = false;
}

GameController::~GameController() {
	// TODO Auto-generated destructor stub
}

void GameController::play() {
	Renderer *renderer = new Renderer();
	if (!renderer->canDraw()){
		printf( "Failed to initialize!\n" );
		delete renderer;
		return;
	}

	View *marioView = new View();
	SDL_Point origin = {200,100};
	marioView->origin = origin;
	renderer->addView(marioView);

	//While application is running
	while( !this->shouldQuit ) {
		this->pollEvents();
		renderer->draw();
	}
	renderer->close();
	delete renderer;
}

void GameController::pollEvents(){
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) {
		if( e.type == SDL_QUIT ) {
			printf("tengo que cerrarlo");
			this->shouldQuit = true;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN){
			cout << "hice click con el mouse \n";
			//Get mouse position
			int x, y;
			SDL_GetMouseState( &x, &y );
			cout << "mouse x: " << x << " y; " << y << endl;
		}
	}
}
