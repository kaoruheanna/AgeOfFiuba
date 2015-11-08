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
#include "../Models/User.h"

class Mensajero;

using namespace std;

class ServerGameController : public EscenarioDelagate, public Mensajero{
public:
	ServerGameController(GameConfiguration *config);
	virtual ~ServerGameController();
	GameConfiguration * getConfig();
	bool inicializado();

	void play();

	virtual void moverEntidad(MobileModel* entity, string username);
	virtual void interactuar(int selectedEntityId, int targetEntityId);

	Escenario* escenario;
	virtual void addMensajero(Mensajero *mensajero);
	void removeMensajero(Mensajero *mesajero);
	void init();

	int userLogin(char* username);
	void setUserActive(char* username);
	void setUserInactive(char* username);

	void addUser(char* username); // No usar => Solo para el play

	//EscenarioDelegate
	virtual void actualizaPersonaje(MobileModel* entity);
	virtual void apareceEntidad(Entity* entidad);
	virtual void desapareceEntidad(Entity* entidad);
private:
	GameConfiguration *config;
	list<Mensajero*> mensajeros;
	// TODO cuando termine de implementar borrar usuarios de escenario
	list<User*> usuarios;

	void loopEscenario();
	void obtenerEventos();
	void enviarEventos();
	void actualizarProtagonista();
	void sleep();

	User* getUserByName(string name);
	bool userExists(char* username);
	bool userActive(char* username);
	Entity* getEntityForUser(int entityId, string username);
	MobileModel* getMobileModelForUser(int entityId, string username);

	bool teamAvailable();

	//Para actualizar
	list<Mensajero*> mensajerosAgregados;

	list<Entity*> recursosAgregados;
	list<Entity*> recursosEliminados;
	bool debeActualizarPersonaje;
	TileCoordinate* moverPersonajeAlPunto;
};

#endif /* CONTROLLERS_SERVERGAMECONTROLLER_H_ */
