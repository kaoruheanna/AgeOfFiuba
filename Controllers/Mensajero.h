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
	virtual void escenarioInicializado(Escenario* escenario){}
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
	virtual void moverProtagonista(SDL_Point point){};

};
#endif /* CONTROLLERS_MENSAJERO_H_ */
