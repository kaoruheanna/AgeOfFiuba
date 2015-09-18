/*
 * GameController.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "../Views/View.h"
#include "../Views/Renderer.h"
#include "../Models/MobileModel.h"
#include <list>
#include "../Configuration/GameConfiguration.h"
#include "../Models/Escenario.h"

using namespace std;

class GameController {
public:
	GameController(GameConfiguration *config);
	virtual ~GameController();
	bool play();

private:
	bool shouldQuit;
	list<View*> views;
	Renderer *renderer;
	Escenario* escenario;
	GameConfiguration *config;

	bool pollEvents();
	void updateWindow();

	void close();
	void sleep();

	// Entre -1 y 1
	float scrollingSpeedX(int x);
	float scrollingSpeedY(int y);
	float scrollingSpeed(int x, int large);

	void moveToPoint(SDL_Point point);
};

#endif /* GAMECONTROLLER_H_ */
