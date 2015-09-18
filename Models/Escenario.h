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
		list<Entity*> getListaEntidades(); // Se usa para agregar las vistas de las entidades

		Escenario(string,int,int,int,int);//construye un mapa vacio
		Escenario();//Este constructor deberia recibir un game config
		~Escenario();
};

#endif
