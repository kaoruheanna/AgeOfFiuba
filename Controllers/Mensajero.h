/*
 * Mensajero.h
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#ifndef CONTROLLERS_MENSAJERO_H_
#define CONTROLLERS_MENSAJERO_H_

#include "ServerGameController.h"
#include "../Configuration/GameConfiguration.h"

class MensajeroCliente {

};


class MensajeroServidor {

};

class Mensajero {
public:
	Mensajero() : client(NULL){};
	virtual ~Mensajero(){};

	//Metodos Cliente->Servidor
	virtual GameConfiguration* obtenerConfiguracion(){return 0;};
	virtual Escenario* obtenerEscenario(){return 0;};
	virtual bool inicializado(){return 0;};

	virtual void addClient(MensajeroCliente* client){};
protected:
	MensajeroCliente* client;
};
#endif /* CONTROLLERS_MENSAJERO_H_ */
