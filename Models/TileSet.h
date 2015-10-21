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

using namespace std;

typedef pair <int,int> Posicion;

// unordered map mejoraria la eficiencia
typedef map<Posicion, Posicion> pointMap;
typedef map<Posicion,int> costMap;

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

	std::list<Posicion> vecinos(Posicion baldosa);
	std::list<Posicion> vecinosTotales(Posicion baldosa); //devuelve todas las posiciones adyacentes
	int distancia(Posicion a, Posicion b); // heuristica a usar en A* (distancia euclideana)
	int heuristica(Posicion a, Posicion b);
	int valorArista(Posicion a, Posicion b);
	bool esVecino(Posicion a, Posicion b);
	bool posicionValida(Posicion posicion);
	bool posicionOcupada(Posicion posicion);
	pointMap caminoMinimo(Posicion origen, Posicion destino, Posicion &destino_real);
	deque<SDL_Point> obtenerCamino(SDL_Point origen, SDL_Point destino);
	Posicion buscarDestinoMasCercano(Posicion origen, Posicion destino);


	virtual ~TileSet();
};

#endif /* TILESET_H_ */
