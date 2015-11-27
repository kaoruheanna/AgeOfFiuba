#include "Escenario.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "TipoPartida/PartidaNormal.h"
#include "TipoPartida/PartidaRey.h"
#include "TipoPartida/PartidaBandera.h"

using namespace std;
const string TAG = "Escenario";

//Crea escenario a partir de la configuracion
void Escenario::init() {
	this->teams.clear();
	this->name = escenarioConfig.getNombre();
	this->inicializacionCorrecta = false;
	this->updated = true;
	this->tipo = NULL;
	if (this->name == "") {
		this->name = "sinNombre";
		Log().Get(TAG, logWARNING) << "El escenario tiene que tener un nombre. Usando nombre "<< this->name;
	}

	if (escenarioConfig.getSizeX() < 1) {
		Log().Get(TAG, logWARNING) << "El escenario " << this->name << " tiene que ser al menos una unidad de ancho. Cargando escenario default.";

	} else if (escenarioConfig.getSizeY() < 1) {
		Log().Get(TAG, logWARNING) << "El escenario " << this->name << " tiene que ser al menos una unidad de alto. Cargando escenario default.";

	} else {
		this->mundo = new Map(escenarioConfig.getSizeX(), escenarioConfig.getSizeY(),TILE_WIDTH_PIXELS, TILE_HEIGHT_PIXELS);
		this->factory = new EntityFactory(this->mundo, tiposConfigList);
		this->protagonista = NULL;
		this->inicializacionCorrecta = true;
		list<EntidadConfig>::iterator itConfigEntidad;
		list<EntidadConfig> configs = escenarioConfig.getEntidades();
		int indice = 0;

		for (itConfigEntidad = configs.begin(); itConfigEntidad != configs.end(); ++itConfigEntidad) {
			Entity* entidad = this->crearEntidadFromConfig(*itConfigEntidad);
			if (entidad == NULL) {
				Log().Get(TAG, logWARNING) << "La entidad N° "<< indice << " del escenario " << this->name<< " no pudo ser creada.";
			} else {
				Log().Get(TAG)<<"Creado "<<entidad->getNombre()<<" de tamaño:"<<entidad->getAnchoBase()<<"x"<<entidad->getAltoBase();
				Log().Get(TAG)<<"En posicon "<<entidad->getPosicion().x<<"x"<<entidad->getPosicion().y<<" y del equipo "<<entidad->getTeamString();


				bool agregado = false;
				agregado = this->guardarEntidad(entidad);

				if (!agregado) {
					delete entidad;
					Log().Get(TAG, logWARNING)<<"La entidad N° "<<indice<<" del escenario "<<this->name<<" no fue agregada al mapa. La misma no puede estar en la misma posicion que otra entidad.";
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

	if(this->inicializacionCorrecta){
		if(escenarioConfig.getTipo().compare(NOMBRE_CENTRO_URBANO) == 0){
			this->tipo = new PartidaNormal();
		} else if(escenarioConfig.getTipo().compare(NOMBRE_REY) == 0){
			this->tipo = new PartidaRey();
		} else if(escenarioConfig.getTipo().compare(NOMBRE_BANDERA) == 0){
			this->tipo = new PartidaBandera();
		}
	}

	if (!this->inicializacionCorrecta && this->mundo != NULL) {
		delete this->mundo;
		this->mundo = NULL;
	}
}

Escenario::Escenario(EscenarioConfig config, list<TipoConfig> tipos) :  escenarioConfig(config), tiposConfigList(tipos){
	init();
}

Escenario::~Escenario(){
	if(this->tipo != NULL){
		delete this->tipo;
		this->tipo = NULL;
	}
}


string Escenario::toString(){
	string n = "Escenario: ";
	return n.append(this-> name);
}

bool Escenario::guardarEntidad(Entity* entidad){
	list<TileCoordinate> tilesOccupied = this->tilesOcupadasPorMobileModels(NULL);

	SDL_Point tilePos = this->mundo->getTileForPosition(entidad->getPosicion());
	if (!this->mundo->puedoConstruir(entidad,tilePos,&tilesOccupied)){
		// no puedo construir porque esta ocupado alguno de los tiles
		return false;
	}

	this->entidadesAgregadas.push_back(entidad);
	this->entidades.push_back(entidad);

	if (entidad->getClass() != MOBILE_MODEL){
		//si no es un mobile model setea los tiles como ocupados
		this->mundo->construirEntidad(entidad,entidad->getPosicion(),&tilesOccupied);
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

Entity* Escenario::entidadClaveParaEquipo(Team team){
	string tipoABuscar = escenarioConfig.getTipo();


	for (auto entity : this->entidades) {
		if(entity->getTeam() == team){
			if(tipoABuscar.compare("debug") == 0 ) {
				return entity;
			}

			if (entity->getNombre().compare(tipoABuscar)==0) {
				return entity;
			}
		}
	}
	return NULL;
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
	for (entidad = this->entidades.begin(); entidad != this->entidades.end(); ++entidad) {
		Entity* entidadReal = (*entidad);
		if (entidadReal->getId() == id) {
			this->mundo->sacarEntidad(entidadReal);
			this->entidades.erase(entidad);
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

std::list<pair<SDL_Point,SDL_Point>> Escenario::getTilesCoordinatesForEntities(list<Entity*> entities){
	list<pair<SDL_Point,SDL_Point>> listaDeTiles;
	if (entities.empty()){
		return listaDeTiles;
	}
	for (Entity* entity: entities){
		listaDeTiles.push_back(this->getTilesCoordinatesForEntity(entity));
	}
	return listaDeTiles;
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
				// Espera hasta 10 loops por que se abra el camino (para que no funcione tan lento el juego)
				model->setPosicion(oldPosition);

				recalculoCount[model->getId()]++;
				if(recalculoCount[model->getId()] > 10){
					model->olvidarCamino();
				}
				/*Log().Get(TAG) << "Recalcular camino para " << model->getId() << " pos: " << oldPosition.x << "," << oldPosition.y;
				SDL_Point destino = model->getFinalDestination();
				model->setPosicion(oldPosition);
				queue <SDL_Point> camino = this->getCaminoForMobileModel(oldPosition,destino,model);
				if(camino.size() <= 0) {
					Log().Get(TAG) << "Modelo " << model->getId() << "esta en "<< model->getPosicion().x <<","<< model->getPosicion().y<<" no puede llegar a " << destino.x << "," << destino.y;
					model->olvidarCamino();
				} else {
					Log().Get(TAG) << "Le seteo un nuevo camino de tamanio " << camino.size();

					//Por las dudas para que no se quede iterando en este else
					recalculoCount[model->getId()]++;
					if(recalculoCount[model->getId()] < 10) {
						model->setPath(camino);
					} else {
						model->olvidarCamino();
					}
				}*/
			} else {
				actualizarPersonajes = true;
			}
		}

		// aca solo se actualiza del lado del server
		this->actualizarTileOcupadaPorPersonaje(model);
	}

	//Interacciones
	for(auto entidad : this->entidades) {
		if (entidad->shouldInteract()){
			entidad->doInteract();
			if (!entidad->esMobileModel()){
				this->delegate->actualizaEntidadEstatica(entidad);
			}

			Entity *otherEntity = entidad->getActiveInteractionEntity();
			if (otherEntity && !otherEntity->esMobileModel()){
				this->delegate->actualizaEntidadEstatica(otherEntity);
			}
		}
	}

	if(this->tipo != NULL){
		this->tipo->calcularEstado(this->entidades);
		list<Team> equiposCambiados = this->tipo->obtenerCambios();
		for(auto equipo: equiposCambiados){
			switch(this->tipo->obtenerEstado(equipo)){
				case PERDIO:
					this->delegate->equipoPerdio(equipo);
					break;
				case GANO:
					this->delegate->equipoGano(equipo);
					break;
				case JUGANDO:
					// Sigue jugando. No avisar.
					break;
			}
		}
		equiposCambiados.clear();
	}

	//Eliminar los que mueren
	// ver si se usa
	for(auto entidad : this->entidades) {
		if (!entidad->estaViva()) {
			this->delegate->desapareceEntidad(entidad);
			this->mundo->sacarEntidad(entidad);
		}
	}
}

void Escenario::actualizarTileOcupadaPorPersonaje(MobileModel *model){
	if(model->estaViva()){
		SDL_Point currentTile = this->mundo->getTileForPosition(model->getPosicion());
		this->tilesWithIds[model->getId()] = TileCoordinate(currentTile.x,currentTile.y);
	} else {
		// Si no esta viva una unidad no ocupa espacio
		this->tilesWithIds.erase(model->getId());
	}
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
	SDL_Point origen = mobileModel->getPosicion();
	queue <SDL_Point> camino = this->getCaminoForMobileModel(origen,destino,mobileModel);
	recalculoCount[mobileModel->getId()] = 0;
	mobileModel->setPath(camino);
}

int Escenario::getDistancia(Entity* from, Entity* to) {
	return this->mundo->getDistancia(from,to);
}

Entity* Escenario::crearEntidadFromConfig(EntidadConfig config) {
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
	for (entidad = this->entidades.begin(); entidad != this->entidades.end(); ++entidad) {
		Entity* entity = (*entidad);
		if(entity->getClass() == MOBILE_MODEL){
			mobileModels.push_back((MobileModel*)entity);
		}
	}
	return mobileModels;
}

list<Entity*> Escenario::getEntidadesEnAreaForJugador(SDL_Point posInicial, SDL_Point posFinal, Team team){
	list<Entity*> listaDeEntidadesMobiles;
	list<Entity*> listaDeEdificios;
	int delta = 4;
	int inicioX = min(posInicial.x,posFinal.x) - delta;
	int inicioY = min(posInicial.y,posFinal.y) - delta;
	int finX = max(posInicial.x,posFinal.x) + delta;
	int finY = max(posInicial.y,posFinal.y) + delta;
	int saltoX = 64;
	int saltoY = 32;
	if ((min(posInicial.x,posFinal.x)-5)%TILE_WIDTH_PIXELS < TILE_WIDTH_PIXELS/2){
		saltoX = 32;
	}
	if ((min(posInicial.y,posFinal.y)-5)%TILE_HEIGHT_PIXELS < TILE_HEIGHT_PIXELS/2){
		saltoX = 16;
	}

	for (int x = inicioX; x <= finX; x += saltoX){
		for (int y = inicioY; y <= finY; y += saltoY){
			Entity* entidad = this->getEntidadEnPosicion({x,y}); //este metodo esta mal TODO
			if (entidad){
				if (entidad->getTeam() == team){
					if (entidad->getClass()==ENTITY && listaDeEdificios.empty()){
						listaDeEdificios.push_back(entidad);
					}
					else{
						bool puedo = true;
						for (Entity* e: listaDeEntidadesMobiles){
							if (e->getId() == entidad->getId()){
								puedo = false;
								break;
							}
						}
						if (puedo)
						listaDeEntidadesMobiles.push_back(entidad);
					}
				}
			}
		}
	}
	if (listaDeEntidadesMobiles.empty()){
		listaDeEntidadesMobiles.clear();
		return listaDeEdificios;
	}
	listaDeEdificios.clear();
	printf("size: %i \n", listaDeEntidadesMobiles.size());
	return listaDeEntidadesMobiles;
}

Entity* Escenario::crearYAgregarNuevaEntidad(const string& tipo, LogicPosition logicPosition,const string& equipo,bool aumentarID) {
	SDL_Point posicion = {logicPosition.first,logicPosition.second};
	SDL_Point tile = this->mundo->getTileForPosition(posicion);

	Entity* newEntity = this->factory->crearEntidad(tipo,tile,equipo,aumentarID);
	if (this->factory->esBuilding(tipo)){
		newEntity->setProgresoConstruccion(0);
	}
	//si no la puede construir porque no hay lugar la elimino y "falla" silenciosamente
	if (!this->puedeConstruirEntidad(newEntity,newEntity->getPosicion())){
		Log().Get(TAG) << "No se pudo crear la entidad porque no habia espacio"<<tipo;
		delete newEntity;
		return NULL;
	}

	this->guardarEntidad(newEntity);
	return newEntity;
}

bool Escenario::posicionValida(SDL_Point posicion){
	TileCoordinate tile = {this->mundo->getTileForPosition(posicion).x,this->mundo->getTileForPosition(posicion).y};
	return this->mundo->tileSet->posicionValida(tile);
}

bool Escenario::posicionVacia(SDL_Point posicion){
	if (this->getEntidadEnPosicion(posicion)==NULL){
		TileCoordinate tile = {this->mundo->getTileForPosition(posicion).x,this->mundo->getTileForPosition(posicion).y};
		return this->mundo->tileSet->posicionOcupada(tile);
	}
	return false;
}

bool Escenario::posicionValidaParaCaminar(SDL_Point posicion){
	if (this->posicionValida(posicion)){
		return true;
		return this->posicionVacia(posicion); //TODO
	}
	return false;
}
