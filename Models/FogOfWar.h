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
#include "../Red/Serializable.h"

enum EstadoDeVisibilidad{
	OCULTO,
	NUBLADO,
	VISIBLE,
};



class FogOfWar: public Serializable {
public:
	FogOfWar();
	FogOfWar(int ancho, int alto);
	void initialice();
	void update(int posicionX, int posicionY);
	EstadoDeVisibilidad getEstado (int posicionX, int posicionY);
	void close();
	~FogOfWar();

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	EstadoDeVisibilidad** matrizDeVisibilidad;
	int ancho;
	int alto;
	list<SDL_Point> TilesVisitados;
	void setInSight(int posicionX, int posicionY);
	void setNublados();
};

#endif /* FOGOFWAR_H_ */
