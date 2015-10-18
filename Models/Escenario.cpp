#include "Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

using namespace std;
const string TAG = "Escenario";


void Escenario::init() {
	map<string, SDL_Point> sizeByType;
	list<TipoConfig>::iterator tipo;
	for (tipo = tiposConfigList.begin(); tipo != tiposConfigList.end(); ++tipo) {
		sizeByType.insert(
				pair<string, SDL_Point>(tipo->getNombre(), {
						tipo->getAnchoBase(), tipo->getAltoBase() }));
	}
	this->name = escenarioConfig.getNombre();
	this->inicializacionCorrecta = false;
	this->updated = true;
	if (this->name == "") {
		this->name = "sinNombre";
		Log().Get("Escenario", logWARNING)
				<< "El escenario tiene que tener un nombre. Usando nombre "
				<< this->name;
	}
	if (escenarioConfig.getSizeX() < 1) {
		Log().Get("Escenario", logWARNING) << "El escenario " << this->name
				<< " tiene que ser al menos una unidad de ancho. Cargando escenario default.";
	} else if (escenarioConfig.getSizeY() < 1) {
		Log().Get("Escenario", logWARNING) << "El escenario " << this->name
				<< " tiene que ser al menos una unidad de alto. Cargando escenario default.";
	} else {
		this->mundo = new Map(escenarioConfig.getSizeX(), escenarioConfig.getSizeY(),
				TILE_WIDTH_PIXELS, TILE_HEIGHT_PIXELS);
		factory = new EntityFactory(this->mundo, sizeByType);
		this->protagonista = factory->crearProtagonista(
				escenarioConfig.getProtagonista().getTipo(),
				{ escenarioConfig.getProtagonista().getX(),
						escenarioConfig.getProtagonista().getY() });
		if (protagonista == NULL) {
			Log().Get("Escenario", logWARNING) << "El escenario " << this->name
					<< " no pudo crear al protagonista. Cargando escenario default.";
		} else if (!this->agregarEntidad(protagonista)) {
			Log().Get("Escenario", logWARNING) << "El escenario " << this->name
					<< " no pudo agregar el protagonista al mapa. Cargando escenario default.";
			delete this->protagonista;
		} else {
			this->inicializacionCorrecta = true;
			list<EntidadConfig>::iterator configEntidad;
			list<EntidadConfig> configs = escenarioConfig.getEntidades();
			int indice = 0;
			for (configEntidad = configs.begin();
					configEntidad != configs.end(); ++configEntidad) {
				Entity* entidad = this->crearEntidad(*configEntidad, false);
				if (entidad == NULL) {
					Log().Get("Escenario", logWARNING) << "La entidad N° "
							<< indice << " del escenario " << this->name
							<< " no pudo ser creada.";
				} else {
					if (!this->construirEntidad(entidad,
							entidad->getPosicion())) {
						delete entidad;
						Log().Get("Escenario", logWARNING) << "La entidad N° "
								<< indice << " del escenario " << this->name
								<< " no fue agregada al mapa. La misma no puede estar en la misma posicion que otra entidad.";
					}
				}
				indice++;
			}
		}
	}

	if (!this->inicializacionCorrecta && this->mundo != NULL) {
		delete this->mundo;
		this->mundo = NULL;
	}
	//Inicializar resources Manager
	this->resourcesManager = new ResourcesManager(this);
	for (int i = 0; i < RESOURCES_QTY; i++){
		const char* resourceName = this->resourcesManager->ResourceTypes()[i];
		string str(resourceName);
		this->protagonista->addResourceToCollect(str);
	}
}

Escenario::Escenario(EscenarioConfig config, list<TipoConfig> tipos) :  escenarioConfig(config), tiposConfigList(tipos){
	init();
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
	}
	return false;
}

MobileModel* Escenario::getProtagonista() {
	return this->protagonista;
}

list<Entity*> Escenario::getListaEntidades(){
	return this->entidades;
}

//Devuelve true si lo pudo borrar
bool Escenario::eliminarRecursoConID(int id) {
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Resource* entidadReal = (Resource*)(*entidad);
		if (entidadReal->id == id) {
				entidades.erase(entidad);
				return true;
		}
	}
	return false;
}

//Devuelve true si cosecho algo
bool Escenario::cosecharEnPosicion(SDL_Point point) {
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Resource* entidadReal = (Resource*)(*entidad);
		SDL_Point position = this->mundo->getTileForPosition(entidadReal->getPosicion());
		if ((position.x == point.x) &&
			(position.y == point.y) &&
			entidadReal->Cosechable) {
				entidadReal->cosechar();
				this->protagonista->didCollectResource(entidadReal->getNombre());
				this->delegate->desapareceEntidad(entidadReal);
				entidades.erase(entidad);
				return true;
		}
	}
	return false;
}

Entity* Escenario::getEntidadEnPosicion(SDL_Point point, bool ignoreCosechables) {
	SDL_Point tile = this->mundo->getTileForPosition(point);
	list<Entity*>::iterator entidad;

	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);

		if (!ignoreCosechables || !(entidadReal->Cosechable)){
			std::pair<SDL_Point,SDL_Point> pair = this->getTilesCoordinatesForEntity(entidadReal);
			int minTileX = pair.first.x;
			int maxTileX = pair.second.x;
			int minTileY = pair.first.y;
			int maxTileY = pair.second.y;
			bool sameX = ((tile.x >= minTileX) && (tile.x <= maxTileX));
			bool sameY = ((tile.y >= minTileY) && (tile.y <= maxTileY));

			if (sameX && sameY){
				return entidadReal;
			}
		}
	}
	return NULL;
}

std::pair<SDL_Point,SDL_Point> Escenario::getTilesCoordinatesForEntity(Entity *entity){
	SDL_Point minTile = this->mundo->getTileForPosition(entity->getPosicion());
	int maxTileX = (minTile.x + entity->getAnchoBase());
	int maxTileY = (minTile.y + entity->getAltoBase());
	SDL_Point maxTile = {maxTileX,maxTileY};
	return std::make_pair(minTile,maxTile);
}

//Actualiza todos los modelos en un nuevo loop
void Escenario::loop() {
	if(this->protagonista->updatePosition()) {
		this->delegate->actualizaPersonaje(this->protagonista);
	}

	SDL_Point point = this->mundo->getTileForPosition(this->protagonista->getPosicion());
	updated = this->cosecharEnPosicion(point);

	list<Entity*> entidadesAInsertar = resourcesManager->InsertResourcesForNewLoopOnMap();
	if (entidadesAInsertar.size() > 0) {
		this->delegate->apareceEntidad(entidadesAInsertar.back());
		this->entidades.splice(this->entidades.end(), entidadesAInsertar);
	}
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
