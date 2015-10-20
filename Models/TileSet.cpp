/*
 * TileSet.cpp
 *
 *  Created on: 08/09/2015
 *      Author: fedelonghi
 */

#include "TileSet.h"

using namespace std;

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

//Devuelve todas las posiciones adyacentes a la baldosa, por las que se puede caminar
std::list<Posicion> TileSet::vecinos(Posicion baldosa){
	list<Posicion>lista_de_vecinos;
	int x = baldosa.first;
	int y = baldosa.second;
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
int TileSet::distancia(Posicion a, Posicion b){
	return sqrt( pow((b.first - a.first),2) + pow((b.second - a.second), 2));
}

//Devuelve el valor de ir de "a" a "b" en la grilla (a y b son vecinos)
//Devuelve -1 si la arista no existe
int TileSet::valorArista(Posicion a, Posicion b){
	if  (!(this->esVecino(a,b))){
		return -1;
	}
	return this->distancia(a,b);
}

bool TileSet::esVecino(Posicion a, Posicion b){
	if (a.first >= 0 and a.second >= 0 and a.first < this->ancho and a.second < this->alto){
		if (b.first >= 0 and b.second >= 0 and b.first < this->ancho and b.second < this->alto){
			if (this->distancia(a,b) >= 1 and (this->distancia(a,b)) < 2){
				if (!(this->matriz[a.first][a.second])or(this->matriz[b.first][b.second])){
					return true;
				}
			}
		}
	}
	return false;
}

pointMap TileSet::caminoMinimo(Posicion origen, Posicion destino, Posicion &destino_real){
	PriorityQueue<Posicion> frontera;
	frontera.put(origen, 0);
	pointMap desde_donde_vino;
	costMap costo_hasta_ahora;
	Posicion posible_destino;

	if (this->matriz[destino.first][destino.second]){
		desde_donde_vino [origen] = origen;
		destino_real = origen;
		return desde_donde_vino;
	}

	desde_donde_vino [origen] = origen;
	costo_hasta_ahora[origen] = 0;

	while (!frontera.empty()) {
		Posicion actual = frontera.get();
		if (actual == destino) {
			destino_real = destino;
			return desde_donde_vino;
		}

		for (Posicion prox : this->vecinos(actual)) {
			int nuevo_costo = costo_hasta_ahora[actual] + this->valorArista(actual, prox);
			if (!costo_hasta_ahora.count(prox) || nuevo_costo < costo_hasta_ahora[prox]) {
				costo_hasta_ahora[prox] = nuevo_costo;
				int prioridad = nuevo_costo + this->distancia(prox, destino); // distancia es la heuristica
				frontera.put(prox, prioridad);
				desde_donde_vino[prox] = actual;
				posible_destino = prox;
				destino_real = posible_destino;
			}
		}
	}
	if (destino_real != destino){
		cout<<"recorro todo"<<endl;
	}
	return desde_donde_vino; // si sale del while es porque no llego al destino, el camino es hasta el lugar mas cercano al destino.
}

deque<SDL_Point> TileSet::obtenerCamino(SDL_Point origen, SDL_Point destino){
	Posicion destino_real;
	cout<<"destino: "<<destino.x<<","<<destino.y<<endl;
	pointMap lugares = this->caminoMinimo({origen.x,origen.y},{destino.x,destino.y}, destino_real);
	cout<<"destino real: "<<destino_real.first<<","<<destino_real.second<<endl;
	deque<SDL_Point> camino;
	Posicion orig = {origen.x, origen.y};

	if (lugares.empty()){ //si no tengo un camino
		camino.push_back(origen);
		return camino; //si no existe el camino minimo devuelvo el origen.
	}
	Posicion actual = destino_real;
	while (actual != orig){
		camino.push_back({actual.first, actual.second});
		actual = lugares[actual];
	}
	return camino;
}
