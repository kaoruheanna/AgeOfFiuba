#include "Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

using namespace std;
const string TAG = "Escenario";


void Escenario::init() {
	this->teams.clear();
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
		factory = new EntityFactory(this->mundo, tiposConfigList);
		this->protagonista = NULL;
		this->inicializacionCorrecta = true;
		list<EntidadConfig>::iterator configEntidad;
		list<EntidadConfig> configs = escenarioConfig.getEntidades();
		int indice = 0;
		for (configEntidad = configs.begin();
				configEntidad != configs.end(); ++configEntidad) {
			Entity* entidad = this->crearEntidad(*configEntidad);
			if (entidad == NULL) {
				Log().Get("Escenario", logWARNING) << "La entidad N° "
						<< indice << " del escenario " << this->name
						<< " no pudo ser creada.";
			} else {
				bool agregado = false;
				if(entidad->getClass() == MOBILE_MODEL){
					agregado = this->agregarEntidad(entidad);
				} else {
					agregado = this->construirEntidad(entidad, entidad->getPosicion());
				}
				if (!agregado) {
					delete entidad;
					Log().Get("Escenario", logWARNING) << "La entidad N° "
							<< indice << " del escenario " << this->name
							<< " no fue agregada al mapa. La misma no puede estar en la misma posicion que otra entidad.";
				} else if(entidad->getTeam() != TEAM_NEUTRAL){
					list<Team>::iterator found = find(this->teams.begin(), this->teams.end(), entidad->getTeam());
					if(found == this->teams.end()){
						// Agregar equipo si no estaba ya en la lista
						this->teams.push_back(entidad->getTeam());
					}
				}
			}
			indice++;
		}
	}

	if (!this->inicializacionCorrecta && this->mundo != NULL) {
		delete this->mundo;
		this->mundo = NULL;
	}
	//Inicializar resources Manager
	this->resourcesManager = new ResourcesManager(this);
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
	this->entidadesAgregadas.push_back(entidad);
	this->entidades.push_back(entidad);
	return true;
}

bool Escenario::construirEntidad(Entity* entidad,SDL_Point origenLogico){
	list<TileCoordinate> tilesOccupied = this->tilesOcupadasPorMobileModels(NULL);

	SDL_Point tilePos = this->mundo->getTileForPosition(origenLogico);
	if (!this->mundo->puedoConstruir(entidad,tilePos,&tilesOccupied)){
		// no puedo construir porque esta ocupado alguno de los tiles
		return false;
	}

	entidad->setPosicion(origenLogico);
	this->agregarEntidad(entidad);

	if (entidad->getClass() != MOBILE_MODEL){
		//si no es un mobile model setea los tiles como ocupados
		this->mundo->construirEntidad(entidad,origenLogico,&tilesOccupied);
	}
	return true;
}

bool Escenario::puedeConstruirEntidad(Entity* entidad,SDL_Point origenLogico){
	SDL_Point tilePos = this->mundo->getTileForPosition(origenLogico);
	list<TileCoordinate> tilesOccupied = this->tilesOcupadasPorMobileModels(NULL);
	return this->mundo->puedoConstruir(entidad,tilePos,&tilesOccupied);
}

list<Entity*> Escenario::getListaEntidades(){
	return this->entidades;
}


Entity* Escenario::entidadConId(int id) {
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = *entidad;
		if (entidadReal->getId() == id) {
			return entidadReal;
		}
	}
	return NULL;
}

//Devuelve true si lo pudo encontrar
bool Escenario::existeRecursoConID(int id) {
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		if((*entidad)->getClass() == RESOURCE){
			Resource* entidadReal = (Resource*)(*entidad);
			if (entidadReal->getId() == id) {
					return true;
			}
		}
	}
	return false;
}

//Devuelve true si lo pudo borrar
bool Escenario::eliminarEntidadConID(int id) {
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		if (entidadReal->getId() == id) {
				this->mundo->sacarEntidad(entidadReal);
				entidades.erase(entidad);
				return true;
		}
	}
	return false;
}

list<Entity*> Escenario::getListaRecursos() {
	list<Entity*>resources;
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		if((*entidad)->getClass() == RESOURCE){
			Resource* entidadReal = (Resource*)(*entidad);
			if (entidadReal->getClass() == RESOURCE) {
					resources.push_back(entidadReal);
			}
		}
	}
	return resources;
}

Entity* Escenario::getEntidadEnPosicion(SDL_Point point) {
	SDL_Point tile = this->mundo->getTileForPosition(point);
	list<Entity*>::iterator entidad;

	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);

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
	return NULL;
}

std::pair<SDL_Point,SDL_Point> Escenario::getTilesCoordinatesForEntity(Entity *entity){
	SDL_Point minTile = this->mundo->getTileForPosition(entity->getPosicion());
	int maxTileX = (minTile.x + entity->getAnchoBase() - 1);
	int maxTileY = (minTile.y + entity->getAltoBase() - 1);
	SDL_Point maxTile = {maxTileX,maxTileY};

//	Log().Get(TAG) << entity->getNombre()<<":";
//	Log().Get(TAG)<<"("<<minTile.x<<","<<minTile.y<<")";
//	Log().Get(TAG)<<"("<<maxTile.x<<","<<maxTile.y<<")";

	return std::make_pair(minTile,maxTile);
}

//Actualiza todos los modelos en un nuevo loop
void Escenario::loop() {

	updated = false;
	bool actualizarPersonajes = false;
	MobileModel* model = NULL;
	list<MobileModel*> mobileModels = this->getMobileModels();
	list<MobileModel*>::iterator found;
	for(found = mobileModels.begin(); found != mobileModels.end(); ++found){
		model = *found;
		SDL_Point oldPosition = model->getPosicion();

		if(model->updatePosition()) {
			SDL_Point aux = this->mundo->getTileForPosition(model->getPosicion());
			TileCoordinate newTile = TileCoordinate(aux.x, aux.y);

			// Si se cruza con otro usuario, lo freno y borro el camino
			if (this->tileOcupadoForEntity(newTile,model)){
				model->setPosicion(oldPosition);
				model->olvidarCamino();
			} else {
				actualizarPersonajes = true;
			}
		}

		// aca solo se actualiza del lado del server
		this->actualizarTileOcupadaPorPersonaje(model);
	}

	//Interacciones
	for(auto entidad : this->entidadesInteractuando) {
		entidad->doInteract();
		this->delegate->actualizaEntidad(entidad);
	}

	//Eliminar los que mueren
	for(auto entidad : this->entidades) {
		if (!entidad->estaViva()) {
			this->delegate->desapareceEntidad(entidad);
			this->mundo->sacarEntidad(entidad);
		}
	}
}

void Escenario::actualizarTileOcupadaPorPersonaje(MobileModel *model){
	SDL_Point currentTile = this->mundo->getTileForPosition(model->getPosicion());
	this->tilesWithIds[model->getId()] = TileCoordinate(currentTile.x,currentTile.y);
}

bool Escenario::tileOcupadoForEntity(TileCoordinate tile,Entity* entity){
	map<int,TileCoordinate>::iterator it;
	for (it = this->tilesWithIds.begin(); it != this->tilesWithIds.end(); it++){
		if (entity->getId() != it->first){
			TileCoordinate tileOcupado = it->second;
			if (tileOcupado == tile){
				return true;
			}
		}
	}
	return false;
}

void Escenario::moveEntityToPos(MobileModel* mobileModel,SDL_Point destino) {
	Log().Get(TAG, logDEBUG) << "Trato de mover la entidad:"<<mobileModel->getNombre()<<"con el id:"<<mobileModel->getId();
	SDL_Point origen = mobileModel->getPosicion();
	queue <SDL_Point> camino = this->getCaminoForMobileModel(origen,destino,mobileModel);
	mobileModel->setPath(camino);
	Log().Get(TAG, logDEBUG) << "El personaje: " << mobileModel->getId() << "se mueve al: " << mobileModel->getDestinationX() << " , " << mobileModel->getDestinationY() << " camino: " << camino.size();
}

int Escenario::getDistancia(Entity* from, Entity* to) {
	return this->mundo->getDistancia(from->getPosicion(),to->getPosicion());
}

Entity* Escenario::crearEntidad(EntidadConfig config) {
	SDL_Point posicion = {config.getX(), config.getY()};
	string tipo = config.getTipo();
	return factory->crearEntidad(tipo, posicion, config.getEquipo());
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

queue<SDL_Point> Escenario::getCaminoForMobileModel(SDL_Point origen, SDL_Point destino,MobileModel *mobileModel){
	list<TileCoordinate> tilesOccupied = this->tilesOcupadasPorMobileModels(mobileModel);
	return this->mundo->obtenerCaminoIgnoringTiles(origen,destino,&tilesOccupied);
}

/*
 * recibe como parametro entidad a ignorar (si tiene)
 * devuelve una lista de tileCoordinate, ocupadas por mobile models
 */
list<TileCoordinate> Escenario::tilesOcupadasPorMobileModels(Entity *entityToIgnore){
	list<TileCoordinate> tilesOccupied;

	map<int,TileCoordinate>::iterator it;
	for (it = this->tilesWithIds.begin(); it != this->tilesWithIds.end(); it++){
		if (!entityToIgnore || (it->first != entityToIgnore->getId())){
			TileCoordinate tile = it->second;
			tilesOccupied.push_back(tile);
		}
	}
	return tilesOccupied;
}

list<TileCoordinate> Escenario::getVecinosLibresForEntity(Entity *entity) {
	list<TileCoordinate> tilesLibres;
	list<TileCoordinate> tilesConMobileModels = this->tilesOcupadasPorMobileModels(NULL);
	//TODO en el cliente no los esta guardando

	std::pair<SDL_Point,SDL_Point> tilesEntity = this->getTilesCoordinatesForEntity(entity);

	//TODO buscar vecino libre
	int minTileX = tilesEntity.first.x;
	int maxTileX = tilesEntity.second.x;
	int minTileY = tilesEntity.first.y;
	int maxTileY = tilesEntity.second.y;

	for (int i = minTileX;i <= maxTileX;i++){
		for (int j = minTileY; j <= maxTileY; j++){
			TileCoordinate tile = TileCoordinate(i,j);
			list<TileCoordinate> auxList = this->mundo->getVecinosLibresForTile(tile,&tilesConMobileModels);
			tilesLibres.merge(auxList);
		}
	}
	//puede tener tiles repetidos, pero creo que no hay problema
	return tilesLibres;
}

// Para manejar varios protagonistas
list<Team> Escenario::getTeams() {
	return this->teams;
}

list<MobileModel*> Escenario::getMobileModels() {
	list<MobileModel*> mobileModels;
	list<Entity*>::iterator entidad;
	for (entidad = entidades.begin(); entidad != entidades.end(); ++entidad) {
		Entity* entity = (*entidad);
		if(entity->getClass() == MOBILE_MODEL){
			mobileModels.push_back((MobileModel*)entity);
		}
	}
	return mobileModels;
}

void Escenario::agregarEntidad(const string& tipo, SDL_Point posicion,const string& equipo) {
	SDL_Point tile = this->mundo->getTileForPosition(posicion);
	Entity* entity = this->factory->crearEntidad(tipo,tile,equipo);
	this->construirEntidad(entity, entity->getPosicion());
}
