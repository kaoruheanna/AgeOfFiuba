/*
 * GameController.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "../Views/Renderer.h"
#include "../Models/MobileModel.h"
#include "../Configuration/GameConfiguration.h"
#include "../Models/Escenario.h"

using namespace std;

class EscenarioView;
class MiniEscenarioView;

class GameController {
public:
	GameController(GameConfiguration *config);
	virtual ~GameController();
	bool play();

private:
	bool shouldQuit;
	EscenarioView *escenarioView;
	MiniEscenarioView *miniEscenarioView;
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
	float scrollingSpeed(int z, int min, int max);
	void moveToPoint(SDL_Point point);
	void initWindowSizes();
	void initMap();
	void initPersonaje();
	void initEntities();

	// Metodos y variables para que no se vaya el scroll del mapa
	SDL_Point intialPointWindowWrapper;
	SDL_Point finalPointWindowWrapper;

	int vertixSlope;
	int middlePoint;

	SDL_Point getMaxVertixForPoint(int yPosition);
	void agregarEntidades(list<Entity*> entidades);
	void actualizarEntidades(list<Entity*> entidades);
	void loopEscenario();
};

#endif /* GAMECONTROLLER_H_ */
