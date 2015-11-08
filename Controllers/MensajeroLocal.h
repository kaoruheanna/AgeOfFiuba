/*
 * Mensajero.h
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#ifndef CONTROLLERS_MENSAJEROLOCAL_H_
#define CONTROLLERS_MENSAJEROLOCAL_H_

#include "ServerGameController.h"
#include "../Configuration/GameConfiguration.h"
#include "Mensajero.h"

class MensajeroLocal : public Mensajero{
public:
	MensajeroLocal();
	MensajeroLocal(ServerGameController* server);
	virtual ~MensajeroLocal();
	virtual void addClient(MensajeroCliente* client);

	//Metodos Cliente->Servidor
	virtual void moverEntidad(MobileModel* entity, string username);
	virtual void interactuar(int selectedEntityId, int targetEntityId);

	//Metodos Servidor->Cliente
	virtual void configEscenario(const string path);
	virtual void apareceRecurso(Resource* recurso);
	virtual void desapareceRecurso(Resource* recurso);
	virtual void actualizaPersonaje(MobileModel* entity);
private:
	ServerGameController* server;
	MensajeroCliente* cliente;
};

#endif /* CONTROLLERS_MENSAJEROLOCAL_H_ */
