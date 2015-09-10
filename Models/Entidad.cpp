#include "Entidad.h"

#include <string>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

Entidad::Entidad(string nombre, SDL_Point posicion, int ancho_base, int alto_base){
	this->nombre = nombre;
	this->posicion = posicion;
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;
}

string Entidad::toString(){
	string n ="Entidad con nombre: ";
	n.append(this->nombre);
	return n;
}

SDL_Point Entidad::getPosicion(){
	return this->posicion;
}

int Entidad::getAnchoBase(){
	return this->ancho_base;
}

int Entidad::getAltoBase(){
	return this->alto_base;
}

void Entidad::setPosicion(SDL_Point nueva_posicion){
	this -> posicion = nueva_posicion;
}
