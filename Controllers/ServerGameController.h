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

class ServerGameController : public EscenarioDelagate, public Mensajero{
public:
	ServerGameController(GameConfiguration *config);
	virtual ~ServerGameController();
	GameConfiguration * getConfig();
	bool inicializado();

	void play();

	virtual void moverProtagonista(MobileModel* entity);

	Escenario* escenario;
	virtual void addMensajero(Mensajero *mensajero);
	bool userExists(char* username);
	bool userActive(char* username);
	void addUser(char* username);
	void init();

	//EscenarioDelegate
	virtual void actualizaPersonaje(MobileModel* entity);
	virtual void apareceEntidad(Entity* entidad);
	virtual void desapareceEntidad(Entity* entidad);
private:
	GameConfiguration *config;
	list<Mensajero*> mensajeros;
	void loopEscenario();
	void actualizarProtagonista();
	void sleep();

	Entity* entidadAparecida;
	bool debeActualizarPersonaje;
	Posicion* moverPersonajeAlPunto;
};

#endif /* CONTROLLERS_SERVERGAMECONTROLLER_H_ */
