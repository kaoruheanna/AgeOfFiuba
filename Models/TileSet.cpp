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

bool TileSet::esVecino(Posicion a, Posicion b){
	//devuelve si la posicion a es vecina a b.
	//si a y b son validas(estan dentro del mapa) y, a o b no estan ocupadas:
	if (this->posicionValida(a) and this->posicionValida(b)
			and !(this->posicionOcupada(a) or this->posicionOcupada(b))){
		float distanciaX = b.first - a.first;
		float distanciaY = b.second - a.second;
		//si la distancia es menor a 1(es decir son adyacentes) y no son la misma posicion.
		if (distanciaX<=1 and distanciaY<=1 and (fabs(distanciaX) + fabs(distanciaY)!=0)){
			if (!(this->posicionOcupada({a.first+distanciaX,a.second})
					or this->posicionOcupada({a.first,a.second+distanciaY}))){
				return true;
			}
		}
	}
	return false;
}

std::list<Posicion> TileSet::vecinos(Posicion baldosa){
	//Devuelve todas las posiciones adyacentes a la baldosa, por las que se puede caminar
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

std::list<Posicion> TileSet::vecinosTotales(Posicion baldosa){
	list<Posicion>lista_de_vecinos;
	int x = baldosa.first;
	int y = baldosa.second;
	for (int i=-1; i<2; i++){
		for (int j=-1; j<2; j++){
			int x_v = x+i;
			int y_v = y+j;
			if (this->posicionValida({x_v,y_v})){
				lista_de_vecinos.push_front({x_v,y_v});
			}
		}
	}
	return lista_de_vecinos;
}

int TileSet::distancia(Posicion a, Posicion b){
	//return sqrt( pow((b.first - a.first),2) + pow((b.second - a.second), 2));
	return abs(b.first - a.first) + abs(b.second - a.second);
}

int TileSet::valorArista(Posicion a, Posicion b){
	if  (!(this->esVecino(a,b))){
		return -1;
	}
	return this->distancia(a,b);
}

//Devuelve el valor heuristico de ir de "a" a "b"
int TileSet::heuristica(Posicion a, Posicion b){
	return sqrt( pow((b.first - a.first),2) + pow((b.second - a.second), 2));
	//return abs(b.first - a.first) + abs(b.second - a.second);
}

//Devuelve el valor de ir de "a" a "b" en la grilla (a y b son vecinos)
//Devuelve -1 si la arista no existe
bool TileSet::posicionValida(Posicion posicion){
	return (posicion.first>=0) and (posicion.first < this->ancho)
			and (posicion.second>=0) and (posicion.second < this->alto);
}

bool TileSet::posicionOcupada(Posicion posicion){
	return this->matriz[posicion.first][posicion.second];
}

pointMap TileSet::caminoMinimo(Posicion origen, Posicion destino, Posicion &destino_real){
	/* destino_real es el destino final del camino. Devuelve un mapa cuya clave es una posicion y
	 * el valor es la posicion desde la cual se llega a la clave. Si la clave es igual al valor,
	 * entonces es la posicion inicial. Si la clave es igual a destino_real es el final del camino.
	 */

	PriorityQueue<Posicion> frontera; //posiciones a evaluar.
	frontera.put(origen, 0);
	pointMap desde_donde_vino;
	costMap costo_hasta_ahora; /*map cuya clave es una posicion y
	 	 	 	 	 	 	 	 su valor es el costo (desde el origen hasta la posicion)
	 	 	 	 	 	 	 	  calculado hasta el momento.*/
	Posicion posible_destino;
	Posicion destino_a = destino;

	//si el destino esta ocupado, entonces no me muevo.
	if (this->posicionOcupada(destino)){
		list<Posicion> vacio;
		destino_a = origen;
		//destino_a = this->buscarDestinoMasCercano(origen, destino, desde_donde_vino);
	}

	desde_donde_vino [origen] = origen;
	costo_hasta_ahora[origen] = 0;

	//mientras hayan posiciones para evaluar:
	while (!frontera.empty()) {
		//obtengo la proxima posicion a evaluar.
		Posicion actual = frontera.get();
		//si la posicion actual es igual al destino encontre el camino minimo.
		if (actual == destino_a) {
			destino_real = destino_a;
			return desde_donde_vino;
		}

		//por cada vecino (= prox):
		for (Posicion prox : this->vecinos(actual)) {
			//calculo el nuevo costo
			int nuevo_costo = costo_hasta_ahora[actual] + this->valorArista(actual, prox);
			//si no hay costo calculado para prox o el nuevo costo es menor a el costo calculado anteriormente:
			if (!costo_hasta_ahora.count(prox) || nuevo_costo < costo_hasta_ahora[prox]) {
				//actualizo costo de prox
				costo_hasta_ahora[prox] = nuevo_costo;
				int prioridad = nuevo_costo + this->heuristica(prox, destino_a); // cuanto mas lejos este mayor es la prioridad
				//agrego a la frontera la posicion
				frontera.put(prox, prioridad);

				desde_donde_vino[prox] = actual;

				posible_destino = prox; //esto creo que esta de mas, seria: destino_real = prox;
				destino_real = posible_destino; //hasta ahora mi destino es prox
			}
		}
	}
	// si sale del while es porque no llego al destino. (recorrio todo el mapa)
	//si el destino_real es distinto al destino al cual quiero llegar:
	//busco el destino mas cercano (hasta ahora devuelve el origen, es decir que no se mueve).
	if (destino_real != destino_a){
		destino_a = this->buscarDestinoMasCercano(origen, destino, desde_donde_vino);
		return this->caminoMinimo(origen,destino_a,destino_real);
	}

	return desde_donde_vino;
}

Posicion TileSet::buscarDestinoMasCercano(Posicion origen, Posicion destino, pointMap camino){
	//return origen;
	Posicion destino_cercano = origen;
	cout<<"destino: "<<destino.first<<","<<destino.second<<endl;
	for (pair <Posicion,Posicion> pos: camino){
		if (this->distancia(pos.first,destino_cercano) < this->distancia(origen,destino_cercano)){
			cout<<"Posicion: "<<pos.first.first<<","<<pos.first.second<<endl;
			destino_cercano = pos.first;
		}
	}
	return destino_cercano;
}

deque<SDL_Point> TileSet::obtenerCamino(SDL_Point origen, SDL_Point destino){
	Posicion destino_real;
	pointMap lugares = this->caminoMinimo({origen.x,origen.y},{destino.x,destino.y}, destino_real);
	deque<SDL_Point> camino;
	Posicion orig = {origen.x, origen.y};

	//todo si no puedo llegar a mi destino no hago nada, habria que buscar la posicion mas cercana.
	if (lugares.empty()){
		return camino; //si no existe el camino minimo devuelvo el camino vacio.
	}
	/*if (destino_real.first != destino.x or destino_real.second != destino.y){
		return camino;
	}*/
	Posicion actual = destino_real;
	while (actual != orig){
		camino.push_back({actual.first, actual.second});
		actual = lugares[actual];
	}
	return camino;
}
