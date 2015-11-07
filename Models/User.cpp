/*
 * User.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: nico
 */

#include "User.h"

User::User(string name) {
	this->name = string(name);
	this->team = NEUTRAL;
	this->active = false;
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
