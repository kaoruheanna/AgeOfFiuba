/*
 * ServerGameController.h
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#ifndef CONTROLLERS_SERVERGAMECONTROLLER_H_
#define CONTROLLERS_SERVERGAMECONTROLLER_H_

#include "../Models/MobileModel.h"
#include "../Models/Escenario.h"
#include "../Configuration/GameConfiguration.h"
#include "MensajeroServidor.h"
#include "Mensajero.h"

class Mensajero;

using namespace std;

class ServerGameController {
public:
	ServerGameController(GameConfiguration *config);
	virtual ~ServerGameController();
	GameConfiguration * getConfig();
	bool inicializado();

	void play();
	void moverProtagonista(SDL_Point point);
	Escenario* escenario;
	virtual void addMensajero(Mensajero *mensajero);
	void init();
private:
	GameConfiguration *config;
	list<Mensajero*> mensajeros;
	void loopEscenario();
	void actualizarProtagonista();
	void sleep();

};

#endif /* CONTROLLERS_SERVERGAMECONTROLLER_H_ */
