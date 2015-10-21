/*
 * Mensajero.h
 *
 *  Created on: Oct 15, 2015
 *      Author: dario
 */

#ifndef CONTROLLERS_MENSAJERO_H_
#define CONTROLLERS_MENSAJERO_H_

#include "../Models/Resource.h"
#include "../Models/MobileModel.h"
#include "../Models/Escenario.h"
#include "../Configuration/GameConfiguration.h"
#include "../Models/FogOfWar.h"

class MensajeroCliente {
public:
	MensajeroCliente(){};
	virtual ~MensajeroCliente(){};

	// Todos
	virtual void ping(){};
	// Metodos Servidor -> Cliente
	virtual void errorDeLogueo(){};
	virtual void configEscenario(const string path, FogOfWar* fog){};
	virtual void recibirForOfWar(FogOfWar* fog){};
	virtual void apareceRecurso(Resource* recurso){};
	virtual void desapareceRecurso(Resource* recurso){};
	virtual void actualizaPersonaje(MobileModel* entity){};
};

class Mensajero : public MensajeroCliente {
public:
	Mensajero();
	virtual ~Mensajero();
	virtual void addClient(MensajeroCliente* client){};

	// Metodos Cliente -> Servidor
	virtual void loguearse(char* nombre){};
	virtual void moverProtagonista(MobileModel* entity){};

};
#endif /* CONTROLLERS_MENSAJERO_H_ */
