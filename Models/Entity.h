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

using namespace std;

class Entity : public Mapeable{
private:
	//int id;
	int ancho_base; //x
	int alto_base; //y


protected:
	string nombre;
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)

public:
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	string getNombre();
	void setPosicion(SDL_Point);

	string toString();

	Entity(string nombre, SDL_Point posicion, int ancho_base, int alto_base);
	Entity(string,int,int);
	~Entity();
};

#endif /* ENTIDAD_H_ */
