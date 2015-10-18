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
#include "../Utils/ResourcesManager.h"
#include "FogOfWar.h"
using namespace std;

class EscenarioDelagate {
public:
	EscenarioDelagate(){};
	virtual ~EscenarioDelagate(){};
	virtual void apareceEntidad(Entity* recurso){};
	virtual void desapareceEntidad(Entity* recurso){};
	virtual void actualizaPersonaje(MobileModel* entity){};
};

class Escenario {
	private:
		string name;
		list<Entity*> entidades;
		MobileModel* protagonista;
		ResourcesManager* resourcesManager;

		bool cosecharEnPosicion(SDL_Point point);
		void init();

	public:
		Map* mundo;
		EntityFactory *factory;
		string toString ();
		bool updated;

		EscenarioDelagate* delegate;

		bool agregarEntidad(Entity*);
		bool construirEntidad(Entity*,SDL_Point);
		void vaciarEntidades();
		void loop();

		SDL_Point getSize();
		Entity* crearEntidad(EntidadConfig config, bool esProtagonista);
		Entity* crearEntidad(const string& tipo, SDL_Point posicion, bool esProtagonista);
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades
		MobileModel* getProtagonista();
		bool eliminarRecursoConID(int id);
		Entity* getEntidadEnPosicion(SDL_Point point, bool ignoreCosechables); //recibe una posicion logica

		bool inicializacionCorrecta;

		EscenarioConfig escenarioConfig;
		list<TipoConfig> tiposConfigList;
		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario(EscenarioConfig escenario, list<TipoConfig> tipos);//Este constructor deberia recibir un game config
		~Escenario();
};

#endif
