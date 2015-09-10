#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <string>
#include <iostream>

#include "Entidad.h"
#include "Mundo.h"

using namespace std;

class Escenario {
	private:
		string name;
		list<Entidad*> entidades;
		Mundo* mundo;
		//entidades
	public:

		string toString ();

		bool agregarEntidad(Entidad*);
		bool construirEntidad(Entidad*,SDL_Point);

		void imprimirEntidades();
		list<Entidad*> getListaEntidades();

		Escenario(string,int,int,int,int);//deberia entregarle un mapa o algo para construir el mapa;
		~Escenario();
};

#endif
