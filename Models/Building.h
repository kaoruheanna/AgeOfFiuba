/*
 * Building.h
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#ifndef MODELS_BUILDING_H_
#define MODELS_BUILDING_H_

#include "Entity.h"
#include "Warrior.h"

class Building: public Entity {
public:
	Building(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Building();
	virtual ~Building();

	virtual void interact(Entity* entity);
	virtual void receiveInteraction(Warrior* entity);

	// Solo se usa para el modo de juego "bandera" => no es necesario pasarlo por red
	Team asesino;
};

#endif /* MODELS_BUILDING_H_ */
