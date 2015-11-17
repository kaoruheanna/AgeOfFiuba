
#ifndef MUNDO_H_
#define MUNDO_H_

#include <SDL2/SDL.h>
#include "Entity.h"
#include "TileSet.h"

class Entity;

class Map {
	private:
		int alto;
		int ancho;

		//matriz de tiles
		int tile_ancho;
		int tile_alto;
		TileSet* tileSet;

	public:
		int getHeight();
		int getWidth();
		int getTileHeight();
		int getTileWidth();
		void setAlto(int);
		void setAncho(int);
		void setAll(int,int,int,int);
		void setTileDimensions(int,int);

		//Recibe dos posiciones logicas, la transforma a tiles y te da la distancia en tiles
		int getDistancia(SDL_Point from,SDL_Point to);

		// Devuelve una posicion de tile al azar o -1, -1 si no hay
		SDL_Point getEmptyTile();
		SDL_Point posicionRelativaRect(SDL_Rect,SDL_Point);

		//recibe la entidad, el tile de inicio, y los tiles ocupados por los mobile models
		bool construirEntidad(Entity*,SDL_Point,list<TileCoordinate> *tilesOccupied);

		// recibe el tileInicio, tileFin y la lista de los tiles ocupados por mobile models
		bool tilesOcupadosPorMobileModels(SDL_Point tileInicio,SDL_Point tileFin,list<TileCoordinate> *tilesOccupied);

		//recibe la entidad, el tile de inicio, y los tiles ocupados por los mobile models
		bool puedoConstruir(Entity* entity,SDL_Point,list<TileCoordinate> *tilesOccupied);

		void sacarEntidad(Entity*);

		SDL_Point getTileForPosition(SDL_Point point);
		SDL_Point getPositionForTile(SDL_Point point,bool centered = false);
		SDL_Point getCenteredPositionForTile(SDL_Point point);

		queue <SDL_Point> obtenerCaminoIgnoringTiles(SDL_Point origen, SDL_Point destino,list<TileCoordinate> *tilesOccupied);
		std::list<TileCoordinate> getVecinosLibresForTile(TileCoordinate tile,list<TileCoordinate> *tilesOccupied);

		Map(int alto, int ancho, int tile_ancho, int tile_alto);
		virtual ~Map();
};


#endif
