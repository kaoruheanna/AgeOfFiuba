/*
 * EscenarioSingleton.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: dario
 */

#include "EscenarioSingleton.h"

EscenarioSingleton::EscenarioSingleton(Escenario* escenario) {
	this->escenario = escenario;
}

EscenarioSingleton::~EscenarioSingleton() {
	this->escenario = NULL;
}

Escenario* EscenarioSingleton::get() {
	return EscenarioSingleton::get(NULL);
}

Escenario* EscenarioSingleton::get(Escenario* escenario) {
	static EscenarioSingleton* escenarioSingleton;

	if (escenario != NULL) {
		escenarioSingleton = new EscenarioSingleton(escenario);
	}
	if (escenarioSingleton == NULL) {
		return NULL;
	}
	return escenarioSingleton->escenario;

}
