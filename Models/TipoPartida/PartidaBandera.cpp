/*
 * PartidaBandera.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: nico
 */

#include "PartidaBandera.h"
#include "../Building.h"

PartidaBandera::PartidaBandera() { }

PartidaBandera::~PartidaBandera() { }

void PartidaBandera::calcularEstado(list<Entity*> entidades) {
	Team apropioAzul = TEAM_NEUTRAL;
	Team apropioRed = TEAM_NEUTRAL;
	Team apropioYellow = TEAM_NEUTRAL;
	Team apropioGreen = TEAM_NEUTRAL;
	// Calcular estado
	list<Entity*>::iterator it;
	for(it = entidades.begin(); it != entidades.end(); ++it){
		Entity* entity = *it;
		if(entity->getNombre().compare(NOMBRE_BANDERA) == 0){
			Building* building = (Building*)entity;
			switch(entity->getTeam()){
				case TEAM_BLUE:
					if(!entity->estaViva()){
						apropioAzul = building->asesino;
					} else {
						apropioAzul = TEAM_BLUE;
					}
					break;
				case TEAM_GREEN:
					if(!entity->estaViva()){
						apropioGreen = building->asesino;
					} else {
						apropioGreen = TEAM_GREEN;
					}
					break;
				case TEAM_RED:
					if(!entity->estaViva()){
						apropioRed = building->asesino;
					} else {
						apropioRed = TEAM_RED;
					}
					break;
				case TEAM_YELLOW:
					if(!entity->estaViva()){
						apropioYellow = building->asesino;
					} else {
						apropioYellow = TEAM_YELLOW;
					}
					break;
			}
		}
	}
	if(apropioAzul != TEAM_BLUE){
		this->perdio(TEAM_BLUE);
	}
	if(apropioRed != TEAM_RED){
		this->perdio(TEAM_RED);
	}
	if(apropioYellow != TEAM_YELLOW){
		this->perdio(TEAM_YELLOW);
	}
	if(apropioGreen != TEAM_GREEN){
		this->perdio(TEAM_GREEN);
	}
	// Cambiar entidades
	for(it = entidades.begin(); it != entidades.end(); ++it){
		Entity* entity = *it;
		if(entity->getTeam() != TEAM_NEUTRAL && entity->estaViva()){
			if(this->obtenerEstado(entity->getTeam()) == PERDIO){
				Team apropio = TEAM_NEUTRAL;
				switch(entity->getTeam()){
					case TEAM_RED:
						apropio = apropioRed;
						break;
					case TEAM_GREEN:
						apropio = apropioGreen;
						break;
					case TEAM_YELLOW:
						apropio = apropioYellow;
						break;
					case TEAM_BLUE:
						apropio = apropioAzul;
				}
				if(apropio == TEAM_NEUTRAL){
					// Si no es de nadie matarlo
					entity->matar();
				} else {
					// Si es de alguien le asigno el nuevo equipo
					entity->setTeam(apropio);
				}
			}
		}
	}
}
