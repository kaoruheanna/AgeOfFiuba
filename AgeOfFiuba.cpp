/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Controllers/GameController.h"
#include "Utils/Log.h"
#include "Configuration/GameConfiguration.h"


int main( int argc, char* args[] )
{
	GameConfiguration *configuration = new GameConfiguration();
	GameController *gameController = new GameController(configuration);
	gameController->play();

	delete gameController;
	delete configuration;
	return 0;
}
