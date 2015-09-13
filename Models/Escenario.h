#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <string>
#include <iostream>

#include "Entity.h"
#include "Map.h"

using namespace std;

class Escenario {
	private:
		string name;
		list<Entity*> entidades;
		Map* mundo;
		//entidades
	public:

		string toString ();

		bool agregarEntidad(Entity*);
		bool construirEntidad(Entity*,SDL_Point);
		void vaciarEntidades();
		void imprimirEntidades();
		list<Entity*> getListaEntidades();

		Escenario(string,int,int,int,int);//deberia entregarle un mapa o algo para construir el mapa;
		~Escenario();
};

#endif
