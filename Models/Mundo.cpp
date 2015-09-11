#include "Mundo.h"

Mundo::Mundo(int alto, int ancho, int tile_alto, int tile_ancho){
	//el ancho y alto del tile se puede determinar con las dimensiones del tile default.
	this -> alto = alto;
	this -> ancho = ancho;
	this -> tile_alto = tile_alto;
	this -> tile_ancho = tile_ancho;
	this -> baldosas = new TileSet(ancho,alto);
}

Mundo::~Mundo(){
}

void Mundo::setAll(int alto, int ancho, int tile_alto, int tile_ancho){
	this -> alto = alto;
	this -> ancho = ancho;
	this -> tile_alto = tile_alto;
	this -> tile_ancho = tile_ancho;
}

/* posicion relativa. Rect refiere a la pantalla,
 * punto_m es un punto en las coordenadas del mapa (coordenadas absolutas).
 * Devuelve punto_v que seria el mismo punto pero respecto del mapa.*/
SDL_Point Mundo::posicionRelativaRect(SDL_Rect rect, SDL_Point punto_m){
	SDL_Point punto_v;
	punto_v.x = punto_m.x - rect.x;
	punto_v.y = punto_m.y - rect.y;
	return punto_v;
}
/*Devuelve la coordenada del tile donde se encuentra el punto.
 * Transforma coordenadas absolutas en logicas*/
PointL Mundo::transformarAL(SDL_Point punto_absoluto){
	PointL punto_logico;
	float xa = punto_absoluto.x;
	float ya = punto_absoluto.y;
	punto_logico.x = (xa/(this->tile_ancho))+(ya/(this->tile_alto));
	punto_logico.y = -(xa/(this->tile_ancho))+(ya/(this->tile_alto));
	return punto_logico;
}

SDL_Point Mundo::transformarLA(PointL punto_logico){
	SDL_Point punto_absoluto;
	float xl = punto_logico.x;
	float yl = punto_logico.y;
	punto_absoluto.y = (this->tile_alto/2.0)*(xl+yl);
	punto_absoluto.x = (this->tile_ancho/2.0)*(xl-yl);
	return punto_absoluto;
}

bool Mundo::puedoConstruir(Entidad* entidad, SDL_Point posicion){
	int size_x = entidad->getAnchoBase();
	int size_y = entidad->getAltoBase();

	//evaluo si el objeto entra en el mapa
	if (posicion.x < 0 || posicion.y < 0){
		return false;
	}else if ((posicion.x + size_x) > this->ancho || (posicion.y + size_y) > this->alto){
		return false;
	}
	//verificar que en todos los tiles que va a ocupar la entidad se pueda construir
	SDL_Point fin = {size_x+posicion.x, size_y+posicion.y};
	return !(this -> baldosas->sectorEstaBloqueado(posicion,fin));
}

bool Mundo::construirEntidad(Entidad* entidad, SDL_Point posicion){
	if (this->puedoConstruir(entidad,posicion)){
		for (int i = 0; i < posicion.x + entidad->getAnchoBase(); i++){
			for (int j = 0; j < posicion.y + entidad->getAnchoBase(); j++){
				SDL_Point tile = {i,j};
				this -> baldosas -> setTileInconstruible(tile);
			}
		}
		return true;
	}
	return false;
}
