/*
 * EscenarioSingleton.h
 *
 *  Created on: Nov 2, 2015
 *      Author: dario
 */

#ifndef UTILS_ESCENARIOSINGLETON_H_
#define UTILS_ESCENARIOSINGLETON_H_

#include "../Models/Escenario.h"

class EscenarioSingleton {
public:
	EscenarioSingleton(Escenario* escenario);
	virtual ~EscenarioSingleton();

	static Escenario* get();
	static Escenario* get(Escenario* escenario);
private:
	Escenario* escenario;
};

#endif /* UTILS_ESCENARIOSINGLETON_H_ */
