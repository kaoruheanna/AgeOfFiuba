/*
 * GameController.cpp
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#include "GameController.h"
#include <stdio.h>
#include <string>
#include "../Views/Renderer.h"

GameController::GameController() {
	// TODO Auto-generated constructor stub
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

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while( !quit ) {
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 ) {
			//User requests quit
			if( e.type == SDL_QUIT ) {
				printf("tengo que cerrar");
				quit = true;
			}
		}
		renderer->draw();
	}
	renderer->close();
	delete renderer;
}
