/*
 * User.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: nico
 */

#include "User.h"

User::User(string name) : User() {
	this->name = string(name);
	this->resourceNames.clear();
	this->resourceNames.push_back("comida");
	this->resourceNames.push_back("madera");
	this->resourceNames.push_back("piedra");
	this->resourceNames.push_back("oro");
}

User::~User() { }

// Getter / Setter
string User::getName() {
	return this->name;
}

Team User::getTeam() {
	return this->team;
}

void User::setTeam(Team team) {
	this->team = team;
}

bool User::isActive() {
	return this->active;
}

void User::setActive(bool active) {
	this->active = active;
}

void User::update(User* newData) {
	this->setActive(newData->isActive());
	this->setTeam(newData->getTeam());
	this->comida = newData->comida;
	this->madera = newData->madera;
	this->piedra = newData->piedra;
	this->oro = newData->oro;
}

// Resources methods

void User::addResourceToCollect(string resourceName) {
	// Do nothing.
}

void User::didCollectResource(string resourceName) {
	if(resourceName.compare("comida") == 0){
		this->comida++;
	} else if(resourceName.compare("madera") == 0){
		this->madera++;
	} else if(resourceName.compare("piedra") == 0){
		this->piedra++;
	}else if(resourceName.compare("oro") == 0){
		this->oro++;
	}
}

list<string> User::getResourcesNames() {
	return this->resourceNames;
}

int User::getValueForResource(string resourceName) {
	if(resourceName.compare("comida") == 0){
		return this->comida;
	} else if(resourceName.compare("madera") == 0){
		return this->madera;
	} else if(resourceName.compare("piedra") == 0){
		return this->piedra;
	}
	else if(resourceName.compare("oro") == 0){
			return this->oro;
	}
	return 0;
}

// Serializable methods
User::User() {
	this->active = false;
	this->team = TEAM_NEUTRAL;
	this->name = "";

	this->comida = 0;
	this->madera = 0;
	this->piedra = 0;
	this->oro = 0;
}

void User::setResourceValues(int comida,int madera,int piedra,int oro){
	this->comida = comida;
	this->madera = madera;
	this->piedra = piedra;
	this->oro = oro;
}

//TODO desharcodear los recursos o harcodearlos en todos lados igual
int User::getTotalBlockCount() {
	return 6;
}

int User::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return (this->name.length() + 1);
	} else if(currentIndex == 1){
		return sizeof(Team);
	} else if(currentIndex == 2){
		return sizeof(bool);
	} else {
		return sizeof(int);
	}
}

void User::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		strcpy((char*)buffer, this->name.c_str());
	} else if(currentIndex == 1){
		memcpy(buffer, &this->team, sizeof(Team));
	} else if(currentIndex == 2){
		memcpy(buffer, &this->active, sizeof(bool));
	} else if(currentIndex == 3){
		memcpy(buffer, &this->comida, sizeof(int));
	} else if(currentIndex == 4){
		memcpy(buffer, &this->madera, sizeof(int));
	} else if(currentIndex == 5){
		memcpy(buffer, &this->piedra, sizeof(int));
	}
}

void User::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		char* auxString = (char*) malloc(strlen((char*)blockData) + 1);
		strcpy(auxString, (char*) blockData);
		this->name = string(auxString);
		free(auxString);
	} else if(currentBlock == 1){
		memcpy(&this->team, blockData, sizeof(Team));
	} else if(currentBlock == 2){
		memcpy(&this->active, blockData, sizeof(bool));
	} else if(currentBlock == 3){
		memcpy(&this->comida, blockData, sizeof(int));
	} else if(currentBlock == 4){
		memcpy(&this->madera, blockData, sizeof(int));
	} else if(currentBlock == 5){
		memcpy(&this->piedra, blockData, sizeof(int));
	}
}
