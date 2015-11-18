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
#include <list>
#include <queue>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <map>
#include <utility>
#include "../GlobalConstants.h"

using namespace std;

class Entity;

// unordered map mejoraria la eficiencia
typedef map<TileCoordinate, TileCoordinate> pointMap;
typedef map<TileCoordinate,int> costMap;

template<typename T, typename Number=int>

struct PriorityQueue {

	typedef pair<Number, T> PQElement;

	priority_queue<PQElement, vector<PQElement>, std::greater<PQElement> > elements;

	inline bool empty() {return elements.empty();}

	inline void put(T item, Number priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

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
	void setTodoConstruible();
	bool setTileConstruible(SDL_Point);
	bool setTileInconstruible(SDL_Point);
	bool sectorEstaBloqueado(SDL_Point, SDL_Point);

	std::list<TileCoordinate> vecinosLibres(TileCoordinate baldosa,list<TileCoordinate> *tilesOccupied);
	std::list<TileCoordinate> vecinosTotales(TileCoordinate baldosa); //devuelve todas las posiciones adyacentes
	int distancia(TileCoordinate a, TileCoordinate b); // heuristica a usar en A* (distancia euclideana)
	int heuristica(TileCoordinate a, TileCoordinate b);
	int valorArista(TileCoordinate a, TileCoordinate b);
	bool esVecinoLibre(TileCoordinate a, TileCoordinate b,list<TileCoordinate> *tilesOccupied);
	bool posicionValida(TileCoordinate posicion);
	bool posicionOcupada(TileCoordinate posicion);
	bool esTileTransitable(TileCoordinate tile, list<TileCoordinate> *tilesOccupied);
	deque<SDL_Point> obtenerCaminoIgnoringTiles(TileCoordinate tileOrigen, TileCoordinate tileDestino,list<TileCoordinate> *tilesOccupied);
	pointMap calcularCaminoMinimoIgnoringTiles(TileCoordinate origen, TileCoordinate destino,TileCoordinate &destino_real, list<TileCoordinate> *tilesOccupied);

	virtual ~TileSet();
};

#endif /* TILESET_H_ */
