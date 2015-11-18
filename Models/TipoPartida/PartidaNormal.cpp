/*
 * PartidaNormal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: nico
 */

#include "PartidaNormal.h"

PartidaNormal::PartidaNormal() { }

PartidaNormal::~PartidaNormal() { }

void PartidaNormal::calcularEstado(list<Entity*> entidades) {
	bool perdioAzul = true;
	bool perdioRed = true;
	bool perdioYellow = true;
	bool perdioGreen = true;
	// Calcular estado
	list<Entity*>::iterator it;
	for(it = entidades.begin(); it != entidades.end(); ++it){
		Entity* entity = *it;
		if(entity->getNombre().compare(NOMBRE_CENTRO_URBANO) == 0){
			switch(entity->getTeam()){
				case TEAM_BLUE:
					perdioAzul = !entity->estaViva();
					break;
				case TEAM_GREEN:
					perdioGreen = !entity->estaViva();
					break;
				case TEAM_RED:
					perdioRed = !entity->estaViva();
					break;
				case TEAM_YELLOW:
					perdioYellow = !entity->estaViva();
					break;
			}
		}
	}
	if(perdioAzul){
		this->perdio(TEAM_BLUE);
	}
	if(perdioRed){
		this->perdio(TEAM_RED);
	}
	if(perdioYellow){
		this->perdio(TEAM_YELLOW);
	}
	if(perdioGreen){
		this->perdio(TEAM_GREEN);
	}
	// Cambiar entidades
	for(it = entidades.begin(); it != entidades.end(); ++it){
		Entity* entity = *it;
		if(entity->getTeam() != TEAM_NEUTRAL){
			if(this->obtenerEstado(entity->getTeam()) == PERDIO){
				entity->matar();
			}
		}
	}
}
