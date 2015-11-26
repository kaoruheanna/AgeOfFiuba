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

class EntidadAMover {
public:
	int entidadId;
	SDL_Point destino;
	string username;
};

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

	bool comenzoPartida; // No usar => Solo para el play
	void addUser(char* username); // No usar => Solo para el play

	//EscenarioDelegate
	virtual void actualizaEntidadEstatica(Entity* entity);
	virtual void actualizaPersonaje(MobileModel* entity);
	virtual void desapareceEntidad(Entity* entidad);
	virtual void construir(Entity* tempEntity);
	virtual void equipoPerdio(Team equipo);
	virtual void equipoGano(Team equipo);
private:
	GameConfiguration *config;
	list<Mensajero*> mensajeros;
	list<User*> usuarios;
	list<EntidadAMover*> entidadesParaMover;

	void loopEscenario();
	void obtenerEventos();
	void enviarEventos();
	void actualizarProtagonista();
	void mandarUsuarios();
	void sleep();
	void resolverMensajeros();

	User* getUserByName(string name);
	User* getUserByTeam(Team team);
	Entity* getEntityForUser(int entityId, string username);
	MobileModel* getMobileModelForUser(int entityId, string username);

	void enviarComienzo();
	void setResourcesGathered();

	//Para actualizar
	list<Mensajero*> mensajerosAgregados;

	list<Entity*> entidadesEstaticasActualizadas;
//	list<Entity*> recursosEliminados;
	bool debeActualizarUsuarios;
	bool debeActualizarPersonaje;
};

#endif /* CONTROLLERS_SERVERGAMECONTROLLER_H_ */
