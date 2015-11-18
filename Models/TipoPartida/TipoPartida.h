/*
 * TipoPartida.h
 *
 *  Created on: Nov 18, 2015
 *      Author: nico
 */

#ifndef MODELS_TIPOPARTIDA_TIPOPARTIDA_H_
#define MODELS_TIPOPARTIDA_TIPOPARTIDA_H_

#include "../Entity.h"
#include "../User.h"
#include <list>

using namespace std;

enum EstadoTeam {
	JUGANDO,
	PERDIO,
	GANO
};

class TipoPartida {
public:
	TipoPartida();
	virtual ~TipoPartida();

	virtual void calcularEstado(list<Entity*> entidades);
	EstadoTeam obtenerEstado(Team equipo);
	list<Team> obtenerCambios();
	void equipoInactivo(Team equipo);
protected:
	void perdio(Team equipo);
	void gano(Team equipo);
private:
	list<Team> equiposJugando;
	list<Team> estadosCambiados;
	bool termino;
};

#endif /* MODELS_TIPOPARTIDA_TIPOPARTIDA_H_ */
