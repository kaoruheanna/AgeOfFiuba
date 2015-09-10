#include <SDL2/SDL.h>
#include "Entidad.h"
#include "TileSet.h"

//PointL es una version del SDL_Point pero con sus coordenadas flotantes.
typedef struct PointL{
	float x;
	float y;
}PointL;

class Mundo {
	private:
		int alto;
		int ancho;
		//matriz de tiles
		//informacion de tiles, alto, ancho
		int tile_ancho;//estos tienen que estar definidos por el tile default
		int tile_alto;
		TileSet* baldosas;


	public:

		int getAlto();
		int getAncho();
		void setAlto(int);
		void setAncho(int);
		void setAll(int,int,int,int);
		void setTileDimensions(int,int);

		SDL_Point posicionRelativaRect(SDL_Rect,SDL_Point);
		PointL transformarAL(SDL_Point);
		SDL_Point transformarLA(PointL);

		bool agregarEntidad(Entidad*);
		bool puedoConstruir(Entidad*,SDL_Point);
		Entidad* sacarEntidad(Entidad*);

		Mundo(int, int, int, int);
		virtual ~Mundo();
};



