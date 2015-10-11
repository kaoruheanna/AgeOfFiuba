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
#include <math.h>
#include <unordered_map>
#include <map>

using namespace std;
/*
// unordered map mejoraria la eficiencia
typedef map<SDL_Point,SDL_Point> pointMap;
typedef map<SDL_Point,int> costMap;


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
};*/

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
	std::list<SDL_Point> obtenerCamino(SDL_Point, SDL_Point);
	std::list<SDL_Point> vecinos(SDL_Point baldosa);

	int distancia(SDL_Point a, SDL_Point b); // heuristica a usar en A* (distancia euclideana)
	int valorArista(SDL_Point a, SDL_Point b);
	bool esVecino(SDL_Point a, SDL_Point b);
	//pointMap caminoMinimo(SDL_Point origen, SDL_Point destino);

	virtual ~TileSet();
};

#endif /* TILESET_H_ */
