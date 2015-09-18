#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <string>
#include <iostream>
#include <map>
#include <SDL2/SDL.h>

#include "Entity.h"
#include "Map.h"
#include "MobileModel.h"
#include "../Configuration/EscenarioConfig.h"
#include "../Configuration/TipoConfig.h"

using namespace std;

class Escenario {
	private:
		string name;
		list<Entity*> entidades;
		MobileModel* protagonista;
		//entidades
	public:
		Map* mundo;

		string toString ();

		bool agregarEntidad(Entity*);
		bool construirEntidad(Entity*,SDL_Point);
		void vaciarEntidades();
		void imprimirEntidades();
		Entity* crearEntidad(EntidadConfig config, map<string, SDL_Point> sizeByType, bool esProtagonista);
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades
		MobileModel* getProtagonista();

		bool inicializacionCorrecta;

		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario(EscenarioConfig escenario, list<TipoConfig> tipos);//Este constructor deberia recibir un game config
		~Escenario();
};

#endif
