/*
 * TipoPartida.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: nico
 */

#include "TipoPartida.h"
#include <algorithm>

TipoPartida::TipoPartida() {
	this->equiposJugando.push_back(TEAM_BLUE);
	this->equiposJugando.push_back(TEAM_GREEN);
	this->equiposJugando.push_back(TEAM_RED);
	this->equiposJugando.push_back(TEAM_YELLOW);

	this->estadosCambiados.clear();
	this->termino = false;
}

TipoPartida::~TipoPartida() {
	this->equiposJugando.clear();
	this->estadosCambiados.clear();
}

void TipoPartida::calcularEstado(list<Entity*> entidades) {
	// Implementar en cada subclase
}

list<Team> TipoPartida::obtenerCambios() {
	list<Team> toReturn(this->estadosCambiados);
	this->estadosCambiados.clear();
	return toReturn;
}

EstadoTeam TipoPartida::obtenerEstado(Team equipo) {
	list<Team>::iterator found = find(this->equiposJugando.begin(),this->equiposJugando.end(), equipo);
	if(found == this->equiposJugando.end()){
		return PERDIO;
	} else {
		if(this->equiposJugando.size() == 1){
			return GANO;
		} else {
			return JUGANDO;
		}
	}
}

void TipoPartida::equipoInactivo(Team equipo) {
	this->perdio(equipo);
}

void TipoPartida::perdio(Team equipo) {
	list<Team>::iterator found = find(this->equiposJugando.begin(),this->equiposJugando.end(), equipo);
	if(found != this->equiposJugando.end()){
		this->equiposJugando.erase(found);
		this->estadosCambiados.push_back(equipo);
	}
	if(!this->termino && (this->equiposJugando.size() == 1)){
		this->gano(*this->equiposJugando.begin());
	}
}

void TipoPartida::gano(Team equipo) {
	this->termino = true;
	list<Team>::iterator it;
	for(it = this->equiposJugando.begin(); it != this->equiposJugando.end(); ++it){
		if(equipo != (*it)){
			this->estadosCambiados.push_back(*it);
		}
	}
	this->equiposJugando.clear();
	this->equiposJugando.push_back(equipo);
	this->estadosCambiados.push_back(equipo);
}
