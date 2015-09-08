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
using namespace std;

class GameController {
public:
	GameController();
	virtual ~GameController();
	void play();

private:
	bool shouldQuit;
	list<View*> views;
	Renderer *renderer;
	MobileModel *model;

	void pollEvents();
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
