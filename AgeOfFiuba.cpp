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
#include "GlobalConstants.h"

#include "Red/Cliente.h"
#include "Red/Servidor.h"
#include "inttypes.h"

#include "Controllers/ClientGameController.h"
#include "Controllers/MensajeroLocal.h"
#include "Controllers/ServerGameController.h"

/*
int main( int argc, char* args[] )
{
	// Detect arguments to startup server or client
	if(argc > 1){
		char* type = args[1];
		bool isServer = (strcmp(type, "-s") == 0);
		bool isClient = (strcmp(type, "-c") == 0);
		if(isClient || isServer){
			char *port = NULL;
			char *ip = NULL;
			for(int index = 2; index < argc; index++){
				if((strcmp(args[index], "-port") == 0) && (index + 1) < argc){
					port = args[index + 1];
				}
				if((strcmp(args[index], "-ip") == 0) && (index + 1) < argc){
					ip = args[index + 1];
				}
			}
			if(port == NULL){
				printf("No port to listen passed. Add one with -port {PORT}\n");
				return -2;
			}
			int portNumber = strtoimax(port, NULL, 10);
			if(isClient){
				if(ip == NULL){
					printf("No server ip to connect passed. Add one with -ip {SERVER IP}\n");
					return -3;
				}
				Cliente* cliente = new Cliente();
				printf("Empieza el cliente con puerto: %i e ip: %s\n", portNumber, ip);
				cliente->empezar(ip, portNumber);
				printf("Termino el cliente\n");
				delete cliente;
			} else {
				Servidor* servidor = new Servidor();
				printf("Empieza el server con puerto: %i\n", portNumber);
				servidor->empezar(portNumber);
				printf("Termino el server\n");
				delete servidor;
			}
		} else {
			printf("Command %s is invalid. Try with -s or -c \n", type);
			return -1;
		}
		return 0;
	}
	// If no arguments then start the YAML game
	bool shouldRestart = false;
	do {
		GameConfiguration *configuration = new GameConfiguration(CONFIG_CUSTOM);
		GameController *gameController = new GameController(configuration);
		shouldRestart = gameController->play();

		delete gameController;
		delete configuration;
	} while(shouldRestart);
	return 0;
}
*/

int main( int argc, char* args[] ) {
	GameConfiguration *configuration = new GameConfiguration(CONFIG_CUSTOM);
	ServerGameController *serverGameController = new ServerGameController(configuration);

	// If no arguments then start the YAML game
	bool shouldRestart = false;
	do {
		serverGameController->play();
		Mensajero* mensajero = new MensajeroLocal(serverGameController);
		ClientGameController *clientGameController = new ClientGameController(mensajero);
		shouldRestart = clientGameController->play();

		delete clientGameController;
		delete mensajero;

	} while(shouldRestart);
	delete serverGameController;
	delete configuration;

	return 0;
}
