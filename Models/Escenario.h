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
		list<Team> teams;
		MobileModel* protagonista;
		ResourcesManager* resourcesManager;

		bool cosecharEnPosicion(SDL_Point point, MobileModel* protagonista);
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
		bool tileOcupadoForUsername(TileCoordinate tile,string username);

		SDL_Point getSize();
		Entity* crearEntidad(EntidadConfig config);
		Entity* crearEntidad(const string& tipo, SDL_Point posicion);
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades
		list<Entity*> getListaRecursos(); // Se usa para agregar las vistas de las entidades
		MobileModel* getProtagonista();
		void setProtagonista(MobileModel* protagonista);
		bool eliminarRecursoConID(int id);
		bool existeRecursoConID(int id);
		Entity* entidadConId(int id);
		Entity* getEntidadEnPosicion(SDL_Point point); //recibe una posicion logica
		std::pair<SDL_Point,SDL_Point> getTilesCoordinatesForEntity(Entity *entity); //devuelve un par con el min y maxTile
		queue<SDL_Point> getCaminoForMobileModel(SDL_Point origen, SDL_Point destino,MobileModel *mobileModel);

		bool inicializacionCorrecta;

		EscenarioConfig escenarioConfig;
		list<TipoConfig> tiposConfigList;
		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario(EscenarioConfig escenario, list<TipoConfig> tipos);//Este constructor deberia recibir un game config
		~Escenario();

		// Manejo de varios personajes
		map<string, MobileModel*> usuarios;
		map<string, TileCoordinate> tilesWithUsers;

		void addUser(char* userName);
		void addUser(char* userName, SDL_Point position);
		void addUser(char* userName, int entityId);
		MobileModel* getUserModel(string username);
		list<Team> getTeams();
};

#endif
