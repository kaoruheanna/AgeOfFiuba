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
#include "../Utils/ResourcesManager.h"

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
	ResourcesManager *resourcesManager;

	bool pollEvents();
	void updateWindow();

	void close();
	void sleep();

	// Entre -1 y 1
	float scrollingSpeedX(int x);
	float scrollingSpeedY(int y);
	float scrollingSpeed(int x, int large);

	void moveToPoint(SDL_Point point);
	void initWindowSizes();

	// Metodos y variables para que no se vaya el scroll del mapa
	SDL_Point intialPointWindowWrapper;
	SDL_Point finalPointWindowWrapper;

	int vertixSlope;
	int middlePoint;

	SDL_Point getMaxVertixForPoint(int yPosition);
	void agregarEntidades(list<Entity*> entidades);
};

#endif /* GAMECONTROLLER_H_ */
