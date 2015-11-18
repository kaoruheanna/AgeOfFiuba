/*
 * entidad.h
 *
 *  Created on: 04/09/2015
 *      Author: fedelonghi
 */

#ifndef ENTIDAD_H_
#define ENTIDAD_H_

#include <string>
#include <SDL2/SDL.h>
#include "../Red/Serializable.h"
#include <list>

class Warrior;
class Building;
class Resource;
class Worker;

using namespace std;

enum EntityType {
	ENTITY,
	MOBILE_MODEL,
	RESOURCE
};

enum Team {
	TEAM_NEUTRAL,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_GREEN,
	TEAM_YELLOW
};

enum EntityState {
	STATE_NORMAL,
	STATE_MOVING,
	STATE_INTERACTING,
	STATE_DIED,
	STATE_CREATING
};

struct PropiedadesTipoUnidad{
	int alcance;
	int vidaInicial;
	int poderAtaque;
	int escudo;
};

class Entity : public Serializable{
private:
	//int id;
	int ancho_base; //x
	int alto_base; //y
	void Init(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);


protected:
	string nombre;
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)
	int id;
	Team team = TEAM_NEUTRAL;
	EntityState state = STATE_NORMAL;
	Entity* activeInteractionEntity;
	int life;
	PropiedadesTipoUnidad propiedadesTipoUnidad;

	// Serializable methods
	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);
	int vidaDescontada(Entity* entity);

public:
	int foodGathered;
		int woodGathered;
		int stoneGathered;
		int goldGathered;
	list<string> creables;
	void setId(int id);
	int getId();
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	string getNombre();
	void setPosicion(SDL_Point);
	virtual bool esJugador();
	virtual string getNombreAMostrar();
	virtual bool admiteNublado();

	void setResourcesToZero();

	EntityState getState();

	void setTeam(Team team);
	Team getTeam();
	string getTeamString();

	bool estaViva();
	void matar();
	void update(Entity* entity);
	//Double Dispatch Intract en forma de visitor
	void stopInteracting();
	void interact(Entity* entity);

	//Metodos de interaccion
	virtual PropiedadesTipoUnidad getPropiedadesTipoUnidad();
	virtual void setPropiedadesTipoUnidad(PropiedadesTipoUnidad propiedades);

	//Solo lectura
	virtual int getAlcance();
	virtual int getPoderAtaque();
	virtual int getLife();
	virtual int getEscudo();

	bool canReach(Entity* entity);

	virtual void doInteract() {};
	virtual void receiveInteraction(Entity* entity) {};
	virtual void receiveInteraction(Building* entity) {};
	virtual void receiveInteraction(Warrior* entity) {};
	virtual void receiveInteraction(Worker *entity) {};

	string toString();
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);

	Entity();
	Entity(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Entity(int id,string,int,int);
	~Entity();

	virtual EntityType getClass();
};

#endif /* ENTIDAD_H_ */
