#include "Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

using namespace std;
const string TAG = "Escenario";

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
	this->updated = true;
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
		factory = new EntityFactory(this->mundo, sizeByType);

		this->protagonista =  factory->crearProtagonista(escenario.getProtagonista().getTipo(),
											{escenario.getProtagonista().getX(), escenario.getProtagonista().getY()});
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
				Entity* entidad = this->crearEntidad(*configEntidad, false);
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
	//Inicializar resources Manager
	this->resourcesManager = new ResourcesManager(this);
	this->niebla = new FogOfWar(this->entidades, this->protagonista, this->mundo);
	this->niebla->initialice();
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

bool Escenario::mapeableForPosition(SDL_Point point) {
	//return this->mundo->mapeableInPosition(point);
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		SDL_Point position = this->mundo->getTileForPosition(entidadReal->getPosicion());
		if ((position.x == point.x) &&
			(position.y == point.y) &&
			(entidadReal != protagonista) &&
			entidadReal->Cosechable) {
				entidades.erase(entidad);
				return true;
		}
	}
	return false;
}

//Actualiza todos los modelos en un nuevo loop
void Escenario::loop() {
	updated = false;
	this->protagonista->updatePosition();
	this->niebla->update();
	SDL_Point point = this->mundo->getTileForPosition(protagonista->getPosicion());
	updated = mapeableForPosition(point);

	entidadesAInsertar = resourcesManager->InsertResourcesForNewLoopOnMap();
	if (entidadesAInsertar.size() > 0) {
		updated = true;
	}
}

list<Entity*> Escenario::getEntidadesAInsertar() {
	return entidadesAInsertar;
}

list<Entity*> Escenario::getEntidadesASacar() {
	return entidadesASacar;
}

Entity* Escenario::crearEntidad(EntidadConfig config, bool esProtagonista) {
	SDL_Point posicion = {config.getX(), config.getY()};
	string tipo = config.getTipo();

	return factory->crearEntidad(tipo, posicion, esProtagonista);
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
