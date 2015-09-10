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

class Entidad{
private:
	//int id;
	//imagen ?
	//pixel_ref_x
	//pixel_ref_y
	string nombre;
	SDL_Point posicion;  // posicion en el mapa (coordenadas logicas)
	int ancho_base; //x
	int alto_base; //y


public:
	SDL_Point getPosicion();
	int getAnchoBase();
	int getAltoBase();
	void setPosicion(SDL_Point);

	string toString();

	Entidad(string,SDL_Point,int,int);
	~Entidad();


};

#endif /* ENTIDAD_H_ */
