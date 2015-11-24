/*
 * FogOfWar.h
 *
 *  Created on: 07/10/2015
 *      Author: die89
 */

#ifndef FOGOFWAR_H_
#define FOGOFWAR_H_

#include <list>
#include "Escenario.h"

enum EstadoDeVisibilidad{
	OCULTO,
	NUBLADO,
	VISIBLE,
};



class FogOfWar {
public:
	FogOfWar(int ancho, int alto);
	void initialice();
	void update(list<Entity*> entidades, Team equipo);
	EstadoDeVisibilidad getEstado (int posicionX, int posicionY);
	void close();
	~FogOfWar();



private:
	EstadoDeVisibilidad** matrizDeVisibilidad;
	int ancho;
	int alto;
	list<SDL_Point> TilesVisitados;
	void setInSight(int posicionX, int posicionY);
	void setNublados();
	void setEntidadesEnemigasInSight(const list<Entity*>& entidades,Team equipo);
	bool isAnyTileVisible(int ancho, int alto, int posicionX,int posicionY);
};

#endif /* FOGOFWAR_H_ */
