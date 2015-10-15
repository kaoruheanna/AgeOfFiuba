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

using namespace std;

class ServerGameController {
public:
	ServerGameController(GameConfiguration *config);
	virtual ~ServerGameController();
	GameConfiguration * getConfig();
	bool inicializado();

	void play();
	Escenario* escenario;

private:
	GameConfiguration *config;
};

#endif /* CONTROLLERS_SERVERGAMECONTROLLER_H_ */
