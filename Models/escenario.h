#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <string>
#include <iostream>

#include "../src/entidad.h"

using namespace std;

class Escenario {
	private:
		string name;
		list<Entidad*> entidades;
		//mundo
		//entidades
	public:

		string toString ();

		bool agregarEntidad(Entidad*);

		void imprimirEntidades();
		list<Entidad*> getListaEntidades();

		Escenario(string);//deberia entregarle un mapa o algo para construir el mapa;
		~Escenario();
};

#endif
