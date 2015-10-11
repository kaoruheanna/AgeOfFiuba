/*
 * TileSet.cpp
 *
 *  Created on: 08/09/2015
 *      Author: fedelonghi
 */

#include "TileSet.h"

TileSet::TileSet(int ancho, int alto) {
	this -> ancho = ancho;
	this -> alto = alto;
	bool** matriz = new bool*[ancho];
	for (int x = 0; x < ancho; ++x){
		matriz[x] = new bool[alto];
	}
	this -> matriz = matriz;
	this->setTodoConstruible();
}

TileSet::~TileSet() {
	//destruir la matriz
}

void TileSet::setTodoConstruible(){
	for (int x = 0; x < this->ancho; ++x){
		for (int y = 0; y < this->alto; ++y){
			this->matriz[x][y] = false;
		}
	}
}

bool TileSet::setTileConstruible(SDL_Point tile_pos){
	if ((tile_pos.x < this -> ancho) && (tile_pos.y < this->alto)){
			this->matriz[tile_pos.x][tile_pos.y] = false;
			return true;
	}
	else return false; //deberia tirar un error
}

bool TileSet::setTileInconstruible(SDL_Point tile_pos){
	if ((tile_pos.x < this -> ancho) && (tile_pos.y < this->alto)){
			this->matriz[tile_pos.x][tile_pos.y] = true;
			return true;
	}
	else return false; //deberia tirar un error
}

/*analiza si el sector comprendido por el rectangulo formado por los puntos,
 * origen y fin hay alguna baldosa que este bloqueada.
 */
bool TileSet::sectorEstaBloqueado(SDL_Point origen, SDL_Point fin){
	int x_inicio = min(origen.x, fin.x);
	int y_inicio = min(origen.y, fin.y);
	int x_fin = max(origen.x, fin.x);
	int y_fin = max(origen.y, fin.y);

	for (int i=x_inicio; i<x_fin; i++){
		for (int j=y_inicio; j<y_fin; j++){
			if (this->matriz[i][j]) return true;
		}
	}

	return false;
}

std::list<SDL_Point> TileSet::obtenerCamino(SDL_Point origen, SDL_Point destino){
	std::list <SDL_Point> camino;
	camino.push_front(origen);
	camino.push_front(destino);
	return camino;
}

//Devuelve todas las posiciones adyacentes a la baldosa, por las que se puede caminar
std::list<SDL_Point> TileSet::vecinos(SDL_Point baldosa){
	list<SDL_Point>lista_de_vecinos;
	int x = baldosa.x;
	int y = baldosa.y;
	for (int i=-1; i<2; i++){
		for (int j=-1; j<2; j++){
			int x_v = x+i;
			int y_v = y+j;
			if (this->esVecino({x_v,y_v},baldosa)){
					lista_de_vecinos.push_front({x_v,y_v});
			}
		}
	}
	return lista_de_vecinos;
}


//Devuelve el valor heuristico de ir de "a" a "b"
int TileSet::distancia(SDL_Point a, SDL_Point b){
	return sqrt( pow((b.x - a.x),2) + pow((b.y - a.y), 2));
}

//Devuelve el valor de ir de "a" a "b" en la grilla (a y b son vecinos)
//Devuelve -1 si la arista no existe
int TileSet::valorArista(SDL_Point a, SDL_Point b){
	if  (!(this->esVecino(a,b))){
		return -1;
	}
	return this->distancia(a,b);
}

bool TileSet::esVecino(SDL_Point a, SDL_Point b){
	if (a.x >= 0 and a.y >= 0 and a.x < this->ancho and a.y < this->alto){
		if (b.x >= 0 and b.y >= 0 and b.x < this->ancho and b.y < this->alto){
			if (this->distancia(a,b) >= 1 and (this->distancia(a,b)) < 2){
				if (!(this->matriz[a.x][a.y])or(this->matriz[b.x][b.y])){
					return true;
				}
			}
		}
	}
	return false;
}

/*
pointMap TileSet::caminoMinimo(SDL_Point origen, SDL_Point destino){
	PriorityQueue<SDL_Point> frontera;
	frontera.put(origen, 0);
	pointMap desde_donde_vino;
	costMap costo_hasta_ahora;
	desde_donde_vino [origen] = origen;
	costo_hasta_ahora[origen] = 0;

	while (true){//(!frontera.empty()) {
		SDL_Point actual = frontera.get();
		if (actual.x == destino.x and actual.y == destino.y) {
			return desde_donde_vino;
		}

		for (SDL_Point prox : this->vecinos(actual)) {
			int nuevo_costo = costo_hasta_ahora[actual] + this->valorArista(actual, prox);
			if (!costo_hasta_ahora.count(prox) || nuevo_costo < costo_hasta_ahora[prox]) {
				costo_hasta_ahora[prox] = nuevo_costo;
				int prioridad = nuevo_costo + this->distancia(prox, destino); // distancia es la heuristica
				//frontera.put(prox, prioridad);
				desde_donde_vino[prox] = actual;
			}
		}
	}
	return desde_donde_vino; // si sale del while es porque no llego al destino, el camino es hasta el lugar mas cercano al destino.
}
*/
