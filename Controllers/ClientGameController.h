/*
 * ClientGameController.h
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#ifndef CONTROLLERS_CLIENTGAMECONTROLLER_H_
#define CONTROLLERS_CLIENTGAMECONTROLLER_H_

#include "../Views/Renderer.h"
#include "../Models/MobileModel.h"
#include "../Models/Escenario.h"
#include "../Configuration/GameConfiguration.h"
#include "Mensajero.h"

using namespace std;

class EscenarioView;
class MiniEscenarioView;

class ClientGameController : public Mensajero{ // Era Mensajero Clientes
public:
	ClientGameController(Mensajero *mensajero);
	virtual ~ClientGameController();
	bool play();
	string username;
private:
	Mensajero* mensajero;

	bool shouldQuit;
	EscenarioView *escenarioView;
	MiniEscenarioView *miniEscenarioView;
	Renderer *renderer;
	Escenario* escenario;
	GameConfiguration* config;
	bool updated;

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
	bool inicializado();
	void setMessageForSelectedEntity(Entity* entity);

	// Metodos y variables para que no se vaya el scroll del mapa
	SDL_Point intialPointWindowWrapper;
	SDL_Point finalPointWindowWrapper;

	int vertixSlope;
	int middlePoint;

	SDL_Point getMaxVertixForPoint(int yPosition);
	void agregarEntidades(list<Entity*> entidades);
	void agregarEntidad(Entity* entidad);
	void actualizarEntidades(list<Entity*> entidades);
	void loopEscenario();

	//Mensajero Cliente
	virtual void errorDeLogueo();
	virtual void configEscenario(const string path);
	virtual void apareceRecurso(Resource* recurso);
	virtual void desapareceRecurso(Resource* recurso);
	virtual void actualizaPersonaje(MobileModel* entity);
};

#endif /* CONTROLLERS_CLIENTGAMECONTROLLER_H_ */
