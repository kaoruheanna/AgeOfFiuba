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

class MensajeroCliente {
public:
	MensajeroCliente(){};
	virtual ~MensajeroCliente(){};
	virtual void apareceRecurso(Resource* recurso){};
	virtual void desapareceRecurso(Resource* recurso){};
	virtual void actualizaPersonaje(MobileModel* entity){};
};

class Mensajero : public MensajeroCliente{
public:
	Mensajero();
	virtual ~Mensajero();
	virtual void addClient(MensajeroCliente* client){};

	//Metodos Cliente->Servidor
	virtual GameConfiguration* obtenerConfiguracion(){return 0;};
	virtual Escenario* obtenerEscenario(){return 0;};
	virtual bool inicializado(){return 0;};
	virtual void moverProtagonista(SDL_Point point){};

};
#endif /* CONTROLLERS_MENSAJERO_H_ */
