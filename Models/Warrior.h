/*
 * Warrior.h
 *
 *  Created on: Oct 25, 2015
 *      Author: dario
 */

#ifndef MODELS_WARRIOR_H_
#define MODELS_WARRIOR_H_

#include "MobileModel.h"

class Warrior: public MobileModel {
public:
	Warrior(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Warrior();
	virtual ~Warrior();
};

#endif /* MODELS_WARRIOR_H_ */
