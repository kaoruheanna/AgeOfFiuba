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
#include "Mapeable.h"
#include "../Red/Serializable.h"

using namespace std;

class Entity : public Mapeable, public Serializable{
private:
	//int id;
	int ancho_base; //x
	int alto_base; //y

protected:
	string nombre;
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)

	// Serializable methods
	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);

public:
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	string getNombre();
	void setPosicion(SDL_Point);
	virtual bool esJugador();
	virtual string getNombreAMostrar();
	string toString();

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);

	Entity(string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Entity(string,int,int);
	~Entity();
};

#endif /* ENTIDAD_H_ */
