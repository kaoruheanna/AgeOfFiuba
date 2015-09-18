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

using namespace std;

class Entity{
private:
	//int id;
	string nombre;
	int ancho_base; //x
	int alto_base; //y

protected:
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)

public:
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	string getNombre();
	void setPosicion(SDL_Point);

	string toString();

	Entity(string,SDL_Point,int,int);
	Entity(string,int,int);
	~Entity();
};

#endif /* ENTIDAD_H_ */
