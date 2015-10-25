
#ifndef MUNDO_H_
#define MUNDO_H_

#include <SDL2/SDL.h>
#include "Entity.h"
#include "TileSet.h"

class Map {
	private:
		int alto;
		int ancho;

		//matriz de tiles
		int tile_ancho;
		int tile_alto;
		TileSet* baldosas;



	public:

		int getHeight();
		int getWidth();
		int getTileHeight();
		int getTileWidth();
		void setAlto(int);
		void setAncho(int);
		void setAll(int,int,int,int);
		void setTileDimensions(int,int);

		// Devuelve una posicion de tile al azar o -1, -1 si no hay
		SDL_Point getEmptyTile();
		SDL_Point posicionRelativaRect(SDL_Rect,SDL_Point);

		bool construirEntidad(Entity*,SDL_Point);
		bool puedoConstruir(Entity*,SDL_Point);
		Entity* sacarEntidad(Entity*);

		SDL_Point getTileForPosition(SDL_Point point);
		SDL_Point getPositionForTile(SDL_Point point);
		SDL_Point getCenteredPositionForTile(SDL_Point point);

		queue<SDL_Point> obtenerCamino(SDL_Point origen, SDL_Point destino);

		Map(int alto, int ancho, int tile_ancho, int tile_alto);
		virtual ~Map();
};


#endif
