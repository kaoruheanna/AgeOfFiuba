#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <SDL2/SDL.h>

#include "Entity.h"
#include "Map.h"
#include "MobileModel.h"
#include "../Configuration/EscenarioConfig.h"
#include "../Configuration/TipoConfig.h"
#include "../Factories/EntityFactory.h"
#include "../Utils/ResourcesManager.h"
#include "FogOfWar.h"
#include "TipoPartida/TipoPartida.h"
using namespace std;

class EscenarioDelagate {
public:
	EscenarioDelagate(){};
	virtual ~EscenarioDelagate(){};
	virtual void apareceEntidad(Entity* recurso) = 0;
	virtual void desapareceEntidad(Entity* recurso) = 0;
	virtual void actualizaPersonaje(MobileModel* entity) = 0;
	virtual void actualizaEntidad(Entity* entity) = 0;
	virtual void equipoPerdio(Team equipo) = 0;
	virtual void equipoGano(Team equipo) = 0;
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
		list<TileCoordinate> tilesOcupadasPorMobileModels(Entity *entityToIgnore);

	public:
		Map* mundo;
		EntityFactory *factory;
		TipoPartida* tipo;
		string toString ();
		bool updated;

		EscenarioDelagate* delegate;
		list<Entity*> entidadesAgregadas;

		bool agregarEntidad(Entity*);
		bool construirEntidad(Entity* entidad,SDL_Point origenLogico);
		bool puedeConstruirEntidad(Entity* entidad,SDL_Point origenLogico);
		void agregarEntidad(const string& tipo, SDL_Point posicion,const string& equipo);

		void vaciarEntidades();
		void loop();
		bool tileOcupadoForEntity(TileCoordinate tile,Entity* entity);
		void moveEntityToPos(MobileModel* mobileModel,SDL_Point destino);
		int getDistancia(Entity* from, Entity* to);

		SDL_Point getSize();
		Entity* crearEntidad(EntidadConfig config);
		Entity* crearEntidad(const string& tipo, SDL_Point posicion);
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades
		list<Entity*> getListaRecursos(); // Se usa para agregar las vistas de las entidades
		bool eliminarEntidadConID(int id);
		bool existeRecursoConID(int id);
		Entity* entidadConId(int id);
		Entity* getEntidadEnPosicion(SDL_Point point); //recibe una posicion logica
		std::pair<SDL_Point,SDL_Point> getTilesCoordinatesForEntity(Entity *entity); //devuelve un par con el min y maxTile
		std::list<pair<SDL_Point,SDL_Point>> getTilesCoordinatesForEntities(list<Entity*> entities);
		queue<SDL_Point> getCaminoForMobileModel(SDL_Point origen, SDL_Point destino,MobileModel *mobileModel);

		list<Entity*> getEntidadesEnAreaForJugador(SDL_Point posInicial, SDL_Point posFinal, Team team);// devuelve todas las entidades dentro del area del rectangulo que pertenecen a un jugador.
		list<TileCoordinate> getVecinosLibresForEntity(Entity *entity);
		void actualizarTileOcupadaPorPersonaje(MobileModel *model);

		bool inicializacionCorrecta;

		EscenarioConfig escenarioConfig;
		list<TipoConfig> tiposConfigList;
		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario(EscenarioConfig escenario, list<TipoConfig> tipos);//Este constructor deberia recibir un game config
		~Escenario();

		// Manejo de varios personajes
		map<int, TileCoordinate> tilesWithIds;

		list<Team> getTeams();
		list<MobileModel*> getMobileModels();


		list<Entity*> entidadesInteractuando;
};

#endif
