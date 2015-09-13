
#ifndef MUNDO_H_
#define MUNDO_H_

#include <SDL2/SDL.h>
#include "Entity.h"
#include "TileSet.h"

//PointL es una version del SDL_Point pero con sus coordenadas flotantes.
typedef struct PointL{
	float x;
	float y;
}PointL;

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
		void setAlto(int);
		void setAncho(int);
		void setAll(int,int,int,int);
		void setTileDimensions(int,int);

		SDL_Point posicionRelativaRect(SDL_Rect,SDL_Point);
		PointL transformarAL(SDL_Point);
		SDL_Point transformarLA(PointL);

		bool construirEntidad(Entity*,SDL_Point);
		bool puedoConstruir(Entity*,SDL_Point);
		Entity* sacarEntidad(Entity*);

		Map(int, int, int, int);
		virtual ~Map();
};


#endif
