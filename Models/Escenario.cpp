#include "Escenario.h"

Escenario::Escenario(string name, int ancho, int alto, int tile_ancho, int tile_alto){
	this -> name = name;
	this -> mundo = new Mundo(ancho, alto, tile_ancho, tile_alto);
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

bool Escenario::construirEntidad(Entidad* entidad,SDL_Point posicion){
	if (this->mundo->construirEntidad(entidad,posicion)){
		entidad->setPosicion(posicion);
		this->agregarEntidad(entidad);
		return true;
	}else return false;
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



