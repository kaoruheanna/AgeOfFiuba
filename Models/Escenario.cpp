#include "Escenario.h"

Escenario::Escenario(string name, int ancho, int alto, int tile_ancho, int tile_alto){
	this -> name = name;
	this -> mundo = new Map(ancho, alto, tile_ancho, tile_alto);
}

Escenario::~Escenario(){
}


string Escenario::toString(){
	string n = "Escenario: ";
	return n.append(this-> name);
}

bool Escenario::agregarEntidad(Entity* entidad){
	list<Entity*>::iterator it;
	it = this->entidades.end();
	this->entidades.insert(it,entidad);
	return true;
}

bool Escenario::construirEntidad(Entity* entidad,SDL_Point posicion){
	if (this->mundo->construirEntidad(entidad,posicion)){
		entidad->setPosicion(posicion);
		this->agregarEntidad(entidad);
		return true;
	}else return false;
}

void Escenario::imprimirEntidades(){
	list<Entity*>::iterator it;
	for (it=this->entidades.begin(); it != this->entidades.end(); ++it){
		Entity* e = *it;
		cout<< e->toString()<<endl;
	}
}

list<Entity*> Escenario::getListaEntidades(){
	return this->entidades;
}


void Escenario::vaciarEntidades(){
	while (!this->entidades.empty()){
		Entity* e = this->entidades.front();
		this->entidades.pop_front();
		delete e;
	}
}
