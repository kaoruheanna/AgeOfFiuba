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

class Warrior;
class Building;

using namespace std;

enum EntityType {
	ENTITY,
	MOBILE_MODEL,
	RESOURCE
};

class Entity : public Serializable{
private:
	//int id;
	int ancho_base; //x
	int alto_base; //y

protected:
	string nombre;
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)
	int id;

	// Serializable methods
	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);

public:
	int getId();
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	string getNombre();
	void setPosicion(SDL_Point);
	virtual bool esJugador();
	virtual string getNombreAMostrar();
	virtual bool admiteNublado();

	//Double Dispatch Intract en forma de visitor
	virtual void interact(Entity* entity) {};
	virtual void receiveInteraction(Entity* entity) {};
	virtual void receiveInteraction(Building* entity) {};
	virtual void receiveInteraction(Warrior* entity) {};

	string toString();
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);

	Entity(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Entity(int id,string,int,int);
	~Entity();

	virtual EntityType getClass();
};

#endif /* ENTIDAD_H_ */
