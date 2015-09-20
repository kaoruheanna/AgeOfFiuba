#include "Map.h"

Map::Map(int alto, int ancho, int tile_ancho, int tile_alto){
	//el ancho y alto del tile se puede determinar con las dimensiones del tile default.
	this -> alto = alto;
	this -> ancho = ancho;
	this -> tile_alto = tile_alto;
	this -> tile_ancho = tile_ancho;
	this -> baldosas = new TileSet(ancho,alto);
}

Map::~Map(){
}

void Map::setAll(int ancho, int alto, int tile_ancho, int tile_alto){
	this -> ancho = ancho;
	this -> alto = alto;
	this -> tile_alto = tile_alto;
	this -> tile_ancho = tile_ancho;
}

int Map::getHeight(){
	return this->alto;
}

int Map::getWidth(){
	return this->ancho;
}

int Map::getTileHeight(){
	return this->tile_alto;
}

int Map::getTileWidth(){
	return this->tile_ancho;
}

/* posicion relativa. Rect refiere a la pantalla,
 * punto_m es un punto en las coordenadas del mapa (coordenadas absolutas).
 * Devuelve punto_v que seria el mismo punto pero respecto del mapa.*/
SDL_Point Map::posicionRelativaRect(SDL_Rect rect, SDL_Point punto_m){
	SDL_Point punto_v;
	punto_v.x = punto_m.x - rect.x;
	punto_v.y = punto_m.y - rect.y;
	return punto_v;
}

/*
/*Devuelve la coordenada del tile donde se encuentra el punto.
 * Transforma coordenadas absolutas en logicas*
PointL Map::transformarAL(SDL_Point punto_absoluto){
	PointL punto_logico;
	float xa = punto_absoluto.x;
	float ya = punto_absoluto.y;
	punto_logico.x = (xa/(this->tile_ancho))+(ya/(this->tile_alto));
	punto_logico.y = -(xa/(this->tile_ancho))+(ya/(this->tile_alto));
	return punto_logico;
}

SDL_Point Map::transformarLA(PointL punto_logico){
	SDL_Point punto_absoluto;
	float xl = punto_logico.x;
	float yl = punto_logico.y;
	punto_absoluto.y = (this->tile_alto/2.0)*(xl+yl);
	punto_absoluto.x = (this->tile_ancho/2.0)*(xl-yl);
	return punto_absoluto;
}
*/
bool Map::puedoConstruir(Entity* entidad, SDL_Point posicion){
	int size_x = entidad->getAnchoBase();
	int size_y = entidad->getAltoBase();

	//evaluo si el objeto entra en el mapa
	if (posicion.x < 0 || posicion.y < 0){
		return false;
	}else if ((posicion.x + size_x) > this->ancho || (posicion.y + size_y) > this->alto){
		return false;
	}
	//verificar que en todos los tiles que va a ocupar la entidad se pueda construir
	SDL_Point fin = {size_x+posicion.x-1, size_y+posicion.y-1};
	return !(this -> baldosas->sectorEstaBloqueado(posicion,fin));
	//return true;
}

bool Map::construirEntidad(Entity* entidad, SDL_Point posicion){
	if (this->puedoConstruir(entidad,posicion)){
		for (int i = 0; i < entidad->getAnchoBase(); i++){
			for (int j = 0; j < entidad->getAnchoBase(); j++){
				SDL_Point tile = {i+posicion.x,j+posicion.y};
				this -> baldosas -> setTileInconstruible(tile);
			}
		}
		return true;
	}
	return false;
}
