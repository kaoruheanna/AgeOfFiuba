#include "Entity.h"

#include <string>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

Entity::Entity(string nombre, SDL_Point posicion, int ancho_base, int alto_base){
	this->nombre = nombre;
	this->posicion = posicion;
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;
}

Entity::Entity(string nombre, int ancho_base, int alto_base){

	this->nombre = nombre;
	this->posicion = {-1,-1}; // posicion invalida hace referencia a que no tiene posicion en el mapa.
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;
}

Entity::~Entity(){

}

string Entity::toString(){
	string n ="Entidad con nombre: ";
	n.append(this->nombre);
	return n;
}

SDL_Point Entity::getPosicion(){
	return this->posicion;
}

int Entity::getAnchoBase(){
	return this->ancho_base;
}

int Entity::getAltoBase(){
	return this->alto_base;
}

void Entity::setPosicion(SDL_Point nueva_posicion){
	this -> posicion = nueva_posicion;
}