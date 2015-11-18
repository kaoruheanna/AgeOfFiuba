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
#include "../Views/FutureBuildingView.h"

using namespace std;

class EscenarioView;
class MiniEscenarioView;

class ClientGameController : public Mensajero, public RendererInteractionDelegate{ // Era Mensajero Clientes
public:
	ClientGameController(Mensajero *mensajero);
	virtual ~ClientGameController();
	bool play();
	bool isAlive();

	//RendererInteractionDelegate
	virtual void leftClickEnEscenario(int x, int y);
	virtual void rightClickEnEscenario(int x, int y);
	virtual void leftMouseUp(int x, int y, int w, int h);
	virtual void createEntityButtonPressed(string entityName);

	string username;

private:
	User* usuario;
	Mensajero* mensajero;
//	Entity* selectedEntity;
	Entity* pendingEntity; //entidad a construir
	FutureBuildingView* futureBuildingView;
	list<Entity*> selectedEntities;

	bool shouldQuit;
	bool serverError;
	EscenarioView *escenarioView;
	MiniEscenarioView *miniEscenarioView;
	Renderer *renderer;
	Escenario* escenario;
	GameConfiguration* config;
	bool updated;
	bool empezoPartida;
	bool mouseDown;
	SDL_Point posInicialMouse;

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
	bool inicializado();
	void setMessageForSelectedEntity(Entity* entity);
	void setMessageForSelectedEntities(list<Entity*> entities);
	list<string> getCreablesListForEntityName(string name);
	void setSelectedEntities(list<Entity*> listaDeEntidades);
	void setCreablesForEntities(list<Entity*> listaDeEntidades);
	void moverMuchasUnidades(SDL_Point destino);
	void moverUnaUnidad(Entity* entidad, SDL_Point destino);
	queue <SDL_Point> obtenerTilesParaMoverse(SDL_Point destino);
	SDL_Point getPosicionPromedioForSelectedEntities();

	// Metodos y variables para que no se vaya el scroll del mapa
	SDL_Point intialPointWindowWrapper;
	SDL_Point finalPointWindowWrapper;

	SDL_Point posicionInicialProtagonista;

	int vertixSlope;
	int middlePoint;

	SDL_Point getMaxVertixForPoint(int yPosition);
	void agregarEntidad(Entity* entidad);
	void agregarPersonaje(MobileModel* personaje);
	void actualizarEntidades(list<Entity*> entidades);
	void loopEscenario();
	void limpiarConstruccion();

	//Mensajero Cliente
	virtual void errorDeLogueo();
	virtual void configEscenario(const string path);
	virtual void apareceRecurso(Resource* recurso);
	virtual void desapareceRecurso(Resource* recurso);
	virtual void actualizarEntidad(Entity* entity);
	virtual void actualizaPersonaje(MobileModel* entity);
	virtual void cambioUsuario(User* user);
	virtual void comenzoPartida();

};

#endif /* CONTROLLERS_CLIENTGAMECONTROLLER_H_ */
