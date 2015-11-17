#include "Map.h"
#include "Entity.h"
#include "../GlobalConstants.h"
#include "../Utils/Log.h"

const string TAG = "Map";

Map::Map(int alto, int ancho, int tile_ancho, int tile_alto){
	//el ancho y alto del tile se puede determinar con las dimensiones del tile default.
	this -> alto = alto;
	this -> ancho = ancho;
	this -> tile_alto = tile_alto;
	this -> tile_ancho = tile_ancho;
	this -> tileSet = new TileSet(ancho,alto);
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

SDL_Point Map::getEmptyTile(){
	SDL_Point tile = { -1, -1 };
	int maxCount = this->ancho * this->alto;
	int count = 0;
	while((count < maxCount) && tile.x == -1){
		tile = { rand() % this->ancho, rand() % this->alto };
		if(this->tileSet->sectorEstaBloqueado(tile, { tile.x + 1, tile.y + 1 })){
			tile = { -1, -1 };
		}
		count ++;
	}
	return tile;
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

bool Map::puedoConstruir(Entity* entidad, SDL_Point tile,list<TileCoordinate> *tilesOccupied){
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
	//no tiene en cuenta los mobileModels
	bool hayEntities = this->tileSet->sectorEstaBloqueado(tile,fin);
	bool hayMobileModels = this->tilesOcupadosPorMobileModels(tile,fin,tilesOccupied);
	return (!hayEntities && !hayMobileModels);
}


bool Map::tilesOcupadosPorMobileModels(SDL_Point tileInicio,SDL_Point tileFin,list<TileCoordinate> *tilesOccupied){
	if (tilesOccupied == NULL){
		return false;
	}

	list<TileCoordinate>::iterator it;
	for (it = tilesOccupied->begin();it != tilesOccupied->end();it++){
		TileCoordinate tileOcupado = *it;
		int x = tileOcupado.first; //del mobile model
		int y = tileOcupado.second; //del mobile model
		bool ocupadoEnX = ((x >= tileInicio.x) && (x <= tileFin.x));
		bool ocupadoEnY = ((y >= tileInicio.y) && (y <= tileFin.y));
		if (ocupadoEnX && ocupadoEnY){
			Log().Get(TAG) << "Tile Ocupado por mobile model";
			return true;
		}
	}
	return false;
}

bool Map::construirEntidad(Entity* entidad, SDL_Point posicion,list<TileCoordinate> *tilesOccupied){
	if (entidad->getClass() == MOBILE_MODEL){
		// el tileset no guarda los tiles ocupados por los mobile models
		return false;
	}
	SDL_Point tilePos = this->getTileForPosition(posicion);
	if (!this->puedoConstruir(entidad,tilePos,tilesOccupied)){
		return false;
	}

	Log().Get(TAG) << "COnstruyo "<<entidad->getNombre();
	for (int i = 0; i < entidad->getAnchoBase(); i++){
		for (int j = 0; j < entidad->getAltoBase(); j++){
			SDL_Point tile = {i+tilePos.x,j+tilePos.y};
			this -> tileSet -> setTileInconstruible(tile);
		}
	}
	return true;
}


void Map::sacarEntidad(Entity* entidad) {
	SDL_Point tilePos = this->getTileForPosition(entidad->getPosicion());
	for (int i = 0; i < entidad->getAnchoBase(); i++){
		for (int j = 0; j < entidad->getAnchoBase(); j++){
			SDL_Point tile = {i+tilePos.x,j+tilePos.y};
			this->tileSet->setTileConstruible(tile);
		}
	}
}

SDL_Point Map::getTileForPosition(SDL_Point point) {
	return { point.x / TILE_SIZE, point.y / TILE_SIZE };
}
SDL_Point Map::getPositionForTile(SDL_Point point, bool centered) {
	int x = point.x * TILE_SIZE;
	int y = point.y * TILE_SIZE;
	if (centered){
		x += TILE_SIZE/2;
		y += TILE_SIZE/2;
	}
	return {x , y};
}

SDL_Point Map::getCenteredPositionForTile(SDL_Point point) {
	return {point.x * TILE_SIZE + (TILE_SIZE/2), point.y * TILE_SIZE + (TILE_SIZE/2)};
}

queue <SDL_Point> Map::obtenerCaminoIgnoringTiles(SDL_Point origen, SDL_Point destino,list<TileCoordinate> *tilesOccupied){
	queue<SDL_Point> camino2;
	SDL_Point punto;

	//transformo las coordenadas a tiles.
//	int offset = (TILE_SIZE/2);
	int offset = 0;
	SDL_Point tileOrigen = this->getTileForPosition({origen.x-offset,origen.y-offset});
	SDL_Point tileDestino = this->getTileForPosition({destino.x-offset,destino.y-offset});

	TileCoordinate origenCoordinate = TileCoordinate(tileOrigen.x,tileOrigen.y);
	TileCoordinate destinoCoordinate = TileCoordinate(tileDestino.x,tileDestino.y);

	deque <SDL_Point> camino = this->tileSet->obtenerCaminoIgnoringTiles(origenCoordinate,destinoCoordinate,tilesOccupied);

	while (!camino.empty()){
		punto = camino.back();
		camino.pop_back();
		punto = this->getCenteredPositionForTile(punto);
		camino2.push(punto);
	}
	return camino2;
}

std::list<TileCoordinate> Map::getVecinosLibresForTile(TileCoordinate tile,list<TileCoordinate> *tilesOccupied) {
	list<TileCoordinate> libres;
	list<TileCoordinate> vecinos = this->tileSet->vecinosTotales(tile);

	list<TileCoordinate>::iterator it;
	for (it = vecinos.begin();it != vecinos.end(); it++){
		TileCoordinate tile = *it;
		if (this->tileSet->posicionValida(tile) && this->tileSet->esTileTransitable(tile,tilesOccupied)){
			libres.push_back(tile);
		}
	}

	return libres;
}

int Map::getDistancia(SDL_Point from,SDL_Point to) {
	//TODO agregar a este cálculo el tamaño de las entidades

	int offset = (TILE_SIZE/2);

	SDL_Point tileOrigen = this->getTileForPosition({from.x-offset,from.y-offset});
	SDL_Point tileDestino = this->getTileForPosition({to.x-offset,to.y-offset});

	int xDiff = abs(tileOrigen.x - tileDestino.x);
	int yDiff = abs(tileOrigen.y - tileDestino.y);

	return xDiff+yDiff;
}
