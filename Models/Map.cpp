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

bool Map::puedoConstruir(Entity* entidad, SDL_Point tile){
	int size_x = entidad->getAnchoBase();
	int size_y = entidad->getAltoBase();

	//evaluo si el objeto entra en el mapa
	if (tile.x < 0 || tile.y < 0){
		return false;
	}else if ((tile.x + size_x) > this->ancho || (tile.y + size_y) > this->alto){
		return false;
	}
	//verificar que en todos los tiles que va a ocupar la entidad se pueda construir
	SDL_Point fin = {size_x+tile.x-1, size_y+tile.y-1};
	return !(this -> baldosas->sectorEstaBloqueado(tile,fin));
}

bool Map::construirEntidad(Entity* entidad, SDL_Point posicion){
	SDL_Point tilePos = this->getTileForPosition(posicion);
	if (this->puedoConstruir(entidad,tilePos)){
		for (int i = 0; i < entidad->getAnchoBase(); i++){
			for (int j = 0; j < entidad->getAnchoBase(); j++){
				SDL_Point tile = {i+tilePos.x,j+tilePos.y};
				this -> baldosas -> setTileInconstruible(tile);
			}
		}
		return true;
	}
	return false;
}

const int TILE_SIZE = 64;

SDL_Point Map::getTileForPosition(SDL_Point point) {
	return { point.x / TILE_SIZE, point.y / TILE_SIZE };
}
SDL_Point Map::getPositionForTile(SDL_Point point) {
	return { point.x * TILE_SIZE, point.y * TILE_SIZE };
}

SDL_Point Map::getCenteredPositionForTile(SDL_Point point) {
	return { point.x * TILE_SIZE+TILE_SIZE/2, point.y * TILE_SIZE+TILE_SIZE/2};
}

queue <SDL_Point> Map::obtenerCamino(SDL_Point origen, SDL_Point destino){
	queue<SDL_Point> camino2;

	//transformo las coordenadas a tiles.
	SDL_Point origenMap = this->getTileForPosition(origen);
	SDL_Point destinoMap = this->getTileForPosition(destino);

	deque <SDL_Point> camino = this->baldosas->obtenerCamino(origenMap, destinoMap);

	SDL_Point punto;

	while (!camino.empty()){
		punto = camino.back();
		camino.pop_back();
		punto = this->getCenteredPositionForTile(punto);
		camino2.push(punto);
	}

	return camino2;

}
