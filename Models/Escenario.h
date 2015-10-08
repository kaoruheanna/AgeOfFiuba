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
#include "../Factories/EntityFactory.h"

using namespace std;

class Escenario {
	private:
		string name;
		list<Entity*> entidades;
		MobileModel* protagonista;

	public:
		Map* mundo;
		EntityFactory *factory;
		string toString ();

		bool agregarEntidad(Entity*);
		bool construirEntidad(Entity*,SDL_Point);
		void vaciarEntidades();
		SDL_Point getSize();
		Entity* crearEntidad(EntidadConfig config, bool esProtagonista);
		Entity* crearEntidad(const string& tipo, SDL_Point posicion, bool esProtagonista);
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades
		MobileModel* getProtagonista();

		bool inicializacionCorrecta;

		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario(EscenarioConfig escenario, list<TipoConfig> tipos);//Este constructor deberia recibir un game config
		~Escenario();
};

#endif
