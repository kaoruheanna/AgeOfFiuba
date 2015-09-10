#include "Escenario.h"

Escenario::Escenario(string name){
	this -> name = name;
}

Escenario::~Escenario(){
}


string Escenario::toString(){
	string n = "Escenario: ";
	return n.append(this-> name);
}

bool Escenario::agregarEntidad(Entidad* entidad){
	list<Entidad*>::iterator it;
	it = this->entidades.end();
	this->entidades.insert(it,entidad);
	return true;
}

void Escenario::imprimirEntidades(){
	list<Entidad*>::iterator it;
	for (it=this->entidades.begin(); it != this->entidades.end(); ++it){
		Entidad* e = *it;
		cout<< e->toString()<<endl;
	}
}

list<Entidad*> Escenario::getListaEntidades(){
	return this->entidades;
}



