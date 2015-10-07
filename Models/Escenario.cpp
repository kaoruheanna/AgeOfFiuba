#include "Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

Escenario::Escenario(EscenarioConfig escenario, list<TipoConfig> tipos) {
	map<string, SDL_Point> sizeByType;
	list<TipoConfig>::iterator tipo;
	for (tipo = tipos.begin(); tipo != tipos.end(); ++tipo){
		sizeByType.insert(
			pair<string, SDL_Point>(tipo->getNombre(), { tipo->getAnchoBase(), tipo->getAltoBase() })
		);
	}
	this->name = escenario.getNombre();
	this->inicializacionCorrecta = false;
	if(this->name == ""){
		this->name = "sinNombre";
		Log().Get("Escenario", logWARNING) << "El escenario tiene que tener un nombre. Usando nombre "<< this->name;
	}
	if(escenario.getSizeX() < 1){
		Log().Get("Escenario", logWARNING) << "El escenario " << this->name << " tiene que ser al menos una unidad de ancho. Cargando escenario default.";
	} else if(escenario.getSizeY() < 1){
		Log().Get("Escenario", logWARNING) << "El escenario " << this->name << " tiene que ser al menos una unidad de alto. Cargando escenario default.";
	} else {
		this->mundo = new Map(escenario.getSizeX(), escenario.getSizeY(),TILE_WIDTH_PIXELS,TILE_HEIGHT_PIXELS);
		Entity* protagonista = this->crearEntidad(escenario.getProtagonista(), sizeByType, true);
		if(protagonista == NULL){
			Log().Get("Escenario", logWARNING) << "El escenario " << this->name << " no pudo crear al protagonista. Cargando escenario default.";
		} else if(!this->agregarEntidad(protagonista)){
			Log().Get("Escenario", logWARNING) << "El escenario " << this->name << " no pudo agregar el protagonista al mapa. Cargando escenario default.";
			delete this->protagonista;
		} else {
			this->inicializacionCorrecta = true;
			list<EntidadConfig>::iterator configEntidad;
			list<EntidadConfig> configs = escenario.getEntidades();
			int indice = 0;
			for (configEntidad = configs.begin(); configEntidad != configs.end(); ++configEntidad){
				Entity* entidad = this->crearEntidad(*configEntidad, sizeByType, false);
				if(entidad == NULL){
					Log().Get("Escenario", logWARNING) << "La entidad N° "<< indice <<" del escenario " << this->name << " no pudo ser creada.";
				} else {
					if(!this->construirEntidad(entidad, entidad->getPosicion())){
						delete entidad;
						Log().Get("Escenario", logWARNING) << "La entidad N° "<< indice <<" del escenario " << this->name << " no fue agregada al mapa. La misma no puede estar en la misma posicion que otra entidad.";
					}
				}
				indice++;
			}
		}
	}
	if(!this->inicializacionCorrecta && this->mundo != NULL){
		delete this->mundo;
		this->mundo = NULL;
	}
}

Escenario::~Escenario(){
}


string Escenario::toString(){
	string n = "Escenario: ";
	return n.append(this-> name);
}

bool Escenario::agregarEntidad(Entity* entidad){
	this->entidades.push_back(entidad);
	return true;
}

bool Escenario::construirEntidad(Entity* entidad,SDL_Point posicion){
	if (this->mundo->construirEntidad(entidad,posicion)){
		entidad->setPosicion(posicion);
		this->agregarEntidad(entidad);
		return true;
	}else return false;
}

MobileModel* Escenario::getProtagonista() {
	return this->protagonista;
}

list<Entity*> Escenario::getListaEntidades(){
	return this->entidades;
}


Entity* Escenario::crearEntidad(EntidadConfig config, map<string, SDL_Point> sizeByType, bool esProtagonista) {
	Entity* entidad = NULL;
	if(config.getTipo() == ""){
		Log().Get("Escenario", logWARNING) << "La entidad tiene que tener un tipo. Descartando entidad.";
	} else {
		SDL_Point posicion = {
			config.getX(), config.getY()
		};
		if(posicion.x < 0 || posicion.x >= this->mundo->getWidth()){
			posicion.x = 0;
			Log().Get("Escenario", logWARNING) << "La entidad " << config.getTipo() << " esta fuera del mapa en ancho. Asumida posicion x = 0.";
		}
		if(posicion.y < 0 || posicion.y >= this->mundo->getHeight()){
			posicion.y = 0;
			Log().Get("Escenario", logWARNING) << "La entidad " << config.getTipo() << " esta fuera del mapa en alto. Asumida posicion y = 0.";
		}
		// Cambia de coordenadas tile a coordenadas mapa "pixel"
		posicion = this->mundo->getPositionForTile(posicion);
		map<string, SDL_Point>::iterator found = sizeByType.find(config.getTipo());
		SDL_Point size = { 1, 1 };
		if(found != sizeByType.end()){
			size = found->second;
		} else {
			Log().Get("Escenario",logWARNING) << "No se encontro el tipo "<< config.getTipo() <<". Usando tamaño de la base 1x1.";
		}
		if(size.x < 1){
			size.x = 1;
			Log().Get("Escenario",logWARNING) << "Ancho de base del tipo "<< config.getTipo() <<" no puede ser menor a una unidad. Usando una unidad de ancho.";
		}
		if(size.y < 1){
			size.y = 1;
			Log().Get("Escenario",logWARNING) << "Alto de base del tipo "<< config.getTipo() <<" no puede ser menor a una unidad. Usando una unidad de alto.";
		}
		if(esProtagonista){
			this->protagonista = new MobileModel(config.getTipo(), posicion, size.x, size.y);
			entidad = this->protagonista;
		} else {
			entidad = new Entity(config.getTipo(), posicion, size.x, size.y);
		}
	}
	return entidad;
}

void Escenario::vaciarEntidades(){
	while (!this->entidades.empty()){
		Entity* e = this->entidades.front();
		this->entidades.pop_front();
		delete e;
	}
}


SDL_Point Escenario::getSize(){
	return this->mundo->getPositionForTile({this->mundo->getHeight(), this->mundo->getWidth()});
}
