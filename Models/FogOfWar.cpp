/* FogOfWar.cpp
 *
 *  Created on: 07/10/2015
 *      Author: die89
 */

#include "FogOfWar.h"
#include "../Utils/Log.h"
const string TAG ="FogOfWar";

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

void FogOfWar::initialice(){
	int i;
	int j;
	for ( i = 0; i < this->ancho; ++i){
		for (j = 0; j < this->alto; ++j){
			this->matrizDeVisibilidad[i][j] = OCULTO;
		}
	}
}

bool FogOfWar::isAnyTileVisible(int ancho, int alto, int posicionX,	int posicionY) {
	for (int x = 0; x < ancho; ++x) {
		for (int y = 0; y < alto; ++y) {
			if (this->getEstado(posicionX + x, posicionY + y) == VISIBLE) {
				return true;
			}
		}
	}
	return false;
}

void FogOfWar::setEntidadesEnemigasInSight(const list<Entity*>& entidades,Team equipo) {
	for (auto entity : entidades) {
		if (entity->getTeam() != equipo) {
			int alto = entity->getAltoBase();
			int ancho = entity->getAnchoBase();
			int posicionX = entity->getPosicion().x / TILE_SIZE;
			int posicionY = entity->getPosicion().y / TILE_SIZE;
			if (this->isAnyTileVisible(ancho, alto, posicionX, posicionY)) {
				for (int x = 0; x < ancho; ++x) {
					for (int y = 0; y < alto; ++y) {
						this->setInSight(posicionX + x, posicionY + y);
					}
				}
			}
		}
	}
}

void FogOfWar::update(list<Entity*> entidades, Team equipo){
	this->setNublados();
	this->TilesVisitados.clear();

	int posicionX = 0;
	int posicionY = 0;

	list<Entity*>::iterator ptrEntidadActual;
	Entity* entidadActual = NULL;
	for (ptrEntidadActual = entidades.begin(); ptrEntidadActual != entidades.end(); ++ptrEntidadActual){
		entidadActual = (*ptrEntidadActual);
		if( entidadActual->getTeam() == equipo ){
			int alto = entidadActual->getAltoBase();
			int ancho = entidadActual->getAnchoBase();
			posicionX = entidadActual->getPosicion().x / TILE_SIZE;
			posicionY = entidadActual->getPosicion().y / TILE_SIZE;
			for (int x = 0; x < ancho; ++x){
				for (int y = 0; y < alto; ++y)
					this->setInSight(posicionX+x,posicionY+y);
			}
		}

	}

	this->setEntidadesEnemigasInSight(entidades, equipo);
}

void FogOfWar::setInSight(int posicionX, int posicionY){
	int coordenadaX;
	int coordenadaY;
	for (coordenadaX = (posicionX-2); coordenadaX <= (posicionX+2); ++coordenadaX){
		for (coordenadaY = (posicionY-2); coordenadaY <= (posicionY+2); ++coordenadaY){
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
	for (int x = 0; x < this->ancho; ++x){
			if (this->matrizDeVisibilidad != NULL)
				delete[] (this->matrizDeVisibilidad[x]);
	}
	if (this->matrizDeVisibilidad != NULL){
		delete[] (this->matrizDeVisibilidad);
		this->matrizDeVisibilidad = NULL;
	}
	this->TilesVisitados.clear();
}

FogOfWar::~FogOfWar() {
	// TODO Auto-generated destructor stub
}
