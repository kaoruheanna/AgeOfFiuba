/*
 * TileSet.h
 *
 *  Created on: 08/09/2015
 *      Author: fedelonghi
 */

#ifndef TILESET_H_
#define TILESET_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

class TileSet {
	/* El tileSet esta constituido por una matriz
	 a la cual se le asigna false si es posible construir en el tile
	 cuya posicion son los indices de la matriz, y true en caso contrario*/
private:
	int ancho;
	int alto;
	bool** matriz;

public:
	TileSet(int,int);
	void imprimir();
	void setTodoConstruible();
	bool setTileConstruible(SDL_Point);
	bool setTileInconstruible(SDL_Point);
	bool sectorEstaBloqueado(SDL_Point, SDL_Point);
	virtual ~TileSet();
};

#endif /* TILESET_H_ */
