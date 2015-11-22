#include "Entity.h"

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "../GlobalConstants.h"
#include "../Utils/Log.h"
#include "../Utils/EscenarioSingleton.h"

using namespace std;

const string TAG ="Entity";

Entity::Entity(){
//	Log().Get(TAG) << "constructor 1";
	//this->Init(0,"",{0,0},1,1);
	this->goldGathered = 0;
	this->woodGathered = 0;
	this->foodGathered = 0;
	this->stoneGathered = 0;
	this->nombre = "";
	this->posicion = {0,0};
	this->ancho_base = 0;
	this->alto_base = 0;
	this->id = -1;
	this->life = 10;
	this->activeInteractionEntity = NULL; //TODO que no sea null
}

Entity::Entity(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base){
//	Log().Get(TAG) << "------constructor 2:"<<nombre;
	this->Init(id,nombre,posicion,ancho_base,alto_base);
	this->goldGathered = 0;
	this->woodGathered = 0;
	this->foodGathered = 0;
	this->stoneGathered = 0;
}

Entity::Entity(int id,string nombre, int ancho_base, int alto_base){
//	Log().Get(TAG) << "constructor 3";
	this->Init(id,nombre,{-1,-1},ancho_base,alto_base);
	this->goldGathered = 0;
	this->woodGathered = 0;
	this->foodGathered = 0;
	this->stoneGathered = 0;
}

void Entity::Init(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base) {
	this->nombre = nombre;
	this->posicion = posicion;
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;
	this->id = id;
	this->life = 100;
	this->activeInteractionEntity = NULL;
}

Entity::~Entity(){

}

string Entity::toString(){
	string n ="Entidad con nombre: ";
	n.append(this->nombre);
	return n;
}

string Entity::getNombre() {
	return this->nombre;
}

void Entity::setId(int id) {
	this->id = id;
}

int Entity::getId() {
	return this->id;
}

//devuelve la posicion logica
SDL_Point Entity::getPosicion(){
	return this->posicion;
}

int Entity::getAnchoBase(){
	return this->ancho_base;
}

int Entity::getAltoBase(){
	return this->alto_base;
}

void Entity::setPosicion(SDL_Point nueva_posicion){
	this -> posicion = nueva_posicion;
}

bool Entity::esJugador() {
	return false;
}

string Entity::getNombreAMostrar() {
	return this->nombre;
}

void Entity::setTeam(Team team) {
	this->team = team;
}

Team Entity::getTeam() {
	return this->team;
}

string Entity::getTeamString(){
	string teamString = "";
	switch (this->team){
		case TEAM_RED:
			teamString = NOMBRE_EQUIPO_RED;
			break;
		case TEAM_BLUE:
			teamString = NOMBRE_EQUIPO_BLUE;
			break;
		case TEAM_GREEN:
			teamString = NOMBRE_EQUIPO_GREEN;
			break;
		case TEAM_YELLOW:
			teamString = NOMBRE_EQUIPO_YELLOW;
			break;
		default:
			teamString = NOMBRE_EQUIPO_NEUTRAL;
	}
	return teamString;
}

bool Entity::estaViva() {
	return this->life > 0;
}

void Entity::matar() {
	this->life = 0;
}

//Serializar
// Metodos de serializacion
int Entity::getTotalBlockCount() {
	return 6;
}

int Entity::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return this->serializeStringSize((char*)this->nombre.c_str());
	} else if (currentIndex == 1){
		return sizeof(SDL_Point);
	} else if(currentIndex == 2){
		return sizeof(int);
	} else if(currentIndex == 3) {
		return sizeof(Team);
	} else if (currentIndex ==4) {
		return sizeof(int);
	} else {
		return sizeof(EntityState);
	}
}

void Entity::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		this->serializeString((char*)this->nombre.c_str(), buffer);
	} else if (currentIndex == 1) {
		memcpy(buffer, &this->posicion, sizeof(SDL_Point));
	} else if(currentIndex == 2){
		memcpy(buffer, &this->id, sizeof(int));
	} else if (currentIndex == 3){
		memcpy(buffer, &this->team, sizeof(Team));
	} else if (currentIndex == 4) {
		memcpy(buffer, &this->life, sizeof(int));
	} else if (currentIndex == 5){
		memcpy(buffer, &this->state, sizeof(EntityState));
	}
}

void Entity::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		char* nombre = this->deserializeString(blockData);
		this->nombre = string(nombre);
		free(nombre);
	} else if(currentBlock == 1){
		memcpy(&this->posicion, blockData, sizeof(SDL_Point));
	} else if (currentBlock == 2) {
		memcpy(&this->id, blockData, sizeof(int));
	} else if (currentBlock == 3) {
		memcpy(&this->team, blockData, sizeof(Team));
	} else if (currentBlock == 4) {
		memcpy(&this->life, blockData, sizeof(int));
	} else if (currentBlock == 5){
		memcpy(&this->state, blockData, sizeof(EntityState));
	}
}

char* Entity::deserializeString(void* blockData) {
	char* toDeserialize = (char*) blockData;
	char* string = (char*) malloc(this->serializeStringSize(toDeserialize));
	this->serializeString(toDeserialize, (void*) string);
	return string;
}

void Entity::serializeString(char* string, void* buffer) {
	strcpy((char*) buffer, string);
}

int Entity::serializeStringSize(char* string) {
	return strlen(string) + 1;
}

void Entity::update(Entity* entity) {
	this->nombre = entity->nombre;
	this->posicion = entity->posicion;
	this->life = entity->life;
	this->state = entity->state;
}

EntityType Entity::getClass() {
	return ENTITY;
}

bool Entity::admiteNublado() {
	return this->getClass()!=MOBILE_MODEL;
}

EntityState Entity::getState() {
	return this->state;
}

void Entity::stopInteracting() {
	activeInteractionEntity = NULL;
	this->state = STATE_NORMAL;
}

void Entity::interact(Entity* entity){
	this->state = STATE_INTERACTING; //TODO si la interaccion falla no deberia interactuar
	//el seteo no es mutuo
	this->activeInteractionEntity = entity;
}

bool Entity::isInteracting(){
	return (this->state == STATE_INTERACTING);
}

bool Entity::canReach(Entity* entity) {
	Escenario* escenario = EscenarioSingleton::get();
	int distancia = escenario->getDistancia(this,entity);
	return (distancia <= this->getAlcance());
}

void Entity::setPropiedadesTipoUnidad(PropiedadesTipoUnidad propiedades) {
	this->propiedadesTipoUnidad = propiedades;
	this->life = this->propiedadesTipoUnidad.vidaInicial;
}

PropiedadesTipoUnidad Entity::getPropiedadesTipoUnidad() {
	return this->propiedadesTipoUnidad;
}

int Entity::getAlcance() {
	return this->propiedadesTipoUnidad.alcance;
}

int Entity::getPoderAtaque() {
	return this->propiedadesTipoUnidad.poderAtaque;
}

int Entity::getVidaInicial(){
	return this->propiedadesTipoUnidad.vidaInicial;
}

int Entity::getLife() {
	return this->life;
}

int Entity::getEscudo() {
	return this->propiedadesTipoUnidad.escudo;
}

int Entity::vidaDescontada(Entity* entity) {
	int escudo = this->getEscudo() ? (rand() % this->getEscudo()) : 0;
	int ataque = entity->getPoderAtaque() ? (rand() % entity->getPoderAtaque()) : 0;
	int vidaDescontada = ataque - escudo;
	return (vidaDescontada > 0) ? vidaDescontada : 0;
}

void Entity::resetResourcesGathered() {
	this->foodGathered = 0;
	this->woodGathered = 0;
	this->goldGathered = 0;
	this->stoneGathered = 0;
}

bool Entity::hasGatheredResources() {
	return ((this->foodGathered > 0) ||
			(this->woodGathered > 0) ||
			(this->goldGathered > 0) ||
			(this->stoneGathered > 0));
}

bool Entity::shouldInteract() {
	return (this->activeInteractionEntity != NULL);
}

Entity* Entity::getActiveInteractionEntity() {
	return this->activeInteractionEntity;
}

bool Entity::esMobileModel(){
	return (this->getClass() == MOBILE_MODEL);
}
