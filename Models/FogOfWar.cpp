/* FogOfWar.cpp
 *
 *  Created on: 07/10/2015
 *      Author: die89
 */

#include "FogOfWar.h"

FogOfWar::FogOfWar(int ancho, int alto){
	this->ancho = ancho;
	this->alto = alto;

	EstadoDeVisibilidad** matriz = new EstadoDeVisibilidad*[ancho];
	for (int x = 0; x < ancho; ++x){
		matriz[x] = new EstadoDeVisibilidad[alto];
	}
	this->matrizDeVisibilidad = matriz;
	this->initialice();
	this->TilesVisitados.clear();
}

FogOfWar::FogOfWar() {
	this->ancho = -1;
	this->alto = -1;
	this->matrizDeVisibilidad = NULL;
	this->TilesVisitados.clear();
}

void FogOfWar::initialice(){
	int i;
	int j;
	for ( i = 0; i < this->ancho; ++i){
		for (j = 0; j < this->alto; ++j){
			this->matrizDeVisibilidad[i][j] = OCULTO;
		}
	}
}

//pasar posicion en TILE!!!!
void FogOfWar::update(int posicionX, int posicionY){
	this->setNublados();
	this->setInSight(posicionX,posicionY);
}

void FogOfWar::setInSight(int posicionX, int posicionY){
	int coordenadaX;
	int coordenadaY;
	for (coordenadaX = (posicionX-1); coordenadaX <= (posicionX+1); ++coordenadaX){
		for (coordenadaY = (posicionY-1); coordenadaY <= (posicionY+1); ++coordenadaY){
			if (((coordenadaX >= 0) && (coordenadaY >= 0)) && ((coordenadaX < ancho) && (coordenadaY < alto))){
			SDL_Point Tile = {coordenadaX,coordenadaY};
			this->TilesVisitados.push_back(Tile);
			this->matrizDeVisibilidad[coordenadaX][coordenadaY] = VISIBLE;
			}
		}
	}
}

void FogOfWar::setNublados(){
	list<SDL_Point>::iterator posicion;
	for (posicion = this->TilesVisitados.begin(); posicion != this->TilesVisitados.end(); ++posicion){
		SDL_Point posicionANublar = (*posicion);
		this->matrizDeVisibilidad[posicionANublar.x][posicionANublar.y] = NUBLADO;
	}
}

EstadoDeVisibilidad FogOfWar::getEstado(int posicionX, int posicionY){
	if (((posicionX >= 0) && (posicionY >= 0)) && ((posicionX < ancho) && (posicionY < alto)))
		return this->matrizDeVisibilidad[posicionX][posicionY];
	return OCULTO;
}

void FogOfWar::close(){
	for (int x = 0; x < ancho; ++x){
			delete (this->matrizDeVisibilidad[x]);
	}
	delete (this->matrizDeVisibilidad);
}

FogOfWar::~FogOfWar() {
	// TODO Auto-generated destructor stub
}

// Serializable methods
int FogOfWar::getTotalBlockCount() {
	return this->alto * this->ancho + 2;
}
int FogOfWar::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex < 2){
		return sizeof(int);
	}
	return sizeof(EstadoDeVisibilidad);
}
void FogOfWar::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex < 2){
		if(currentIndex == 0){
			memcpy(buffer, &this->alto, sizeof(int));
		} else {
			memcpy(buffer, &this->ancho, sizeof(int));
		}
		return;
	}
	currentIndex = currentIndex - 2;
	int fila = currentIndex % this->ancho;
	int columna = currentIndex - (fila * this->ancho);
	EstadoDeVisibilidad estado = this->matrizDeVisibilidad[fila][columna];
	memcpy(buffer, &estado, sizeof(EstadoDeVisibilidad));
}
void FogOfWar::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock < 2){
		if(currentBlock == 0){
			memcpy(&this->alto, blockData, sizeof(int));
		} else {
			memcpy(&this->ancho, blockData, sizeof(int));
			// Init array
			EstadoDeVisibilidad** matriz = new EstadoDeVisibilidad*[ancho];
			for (int x = 0; x < ancho; ++x){
				matriz[x] = new EstadoDeVisibilidad[alto];
			}
			this->matrizDeVisibilidad = matriz;
			this->initialice();
			this->TilesVisitados.clear();
		}
		return;
	}
	currentBlock = currentBlock - 2;
	int fila = currentBlock % this->ancho;
	int columna = currentBlock - (fila * this->ancho);
	EstadoDeVisibilidad estado;
	memcpy(&estado, blockData, sizeof(EstadoDeVisibilidad));
	this->matrizDeVisibilidad[fila][columna] = estado;
}
