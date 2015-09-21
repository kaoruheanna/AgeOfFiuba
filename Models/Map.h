
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
		//tile default -> es la baldosa que se va a dibujar en todo el mapa.
		int tile_ancho;//estos tienen que estar definidos por el tile default
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

		SDL_Point posicionRelativaRect(SDL_Rect,SDL_Point);

		bool construirEntidad(Entity*,SDL_Point);
		bool puedoConstruir(Entity*,SDL_Point);
		Entity* sacarEntidad(Entity*);

		SDL_Point getTileForPosition(SDL_Point point);
		SDL_Point getPositionForTile(SDL_Point point);

		Map(int alto, int ancho, int tile_ancho, int tile_alto);
		virtual ~Map();
};


#endif
