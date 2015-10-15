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

class ClientGameController;


class MensajeroLocal : public Mensajero{
public:
	MensajeroLocal(ServerGameController* server);
	virtual ~MensajeroLocal();

	//Metodos Cliente->Servidor
	virtual GameConfiguration* obtenerConfiguracion();
	virtual Escenario* obtenerEscenario();
	virtual bool inicializado();

	//virtual void addClient(ClientGameController* client);
private:
	ServerGameController* server;
	ClientGameController* client;
};

#endif /* CONTROLLERS_MENSAJEROLOCAL_H_ */
