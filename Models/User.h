/*
 * User.h
 *
 *  Created on: Nov 6, 2015
 *      Author: nico
 */

#ifndef MODELS_USER_H_
#define MODELS_USER_H_

#include "Entity.h"
#include <string>

using namespace std;

class User {
public:
	User(string name);
	virtual ~User();

	// Getter / Setter
	string getName();

	Team getTeam();
	void setTeam(Team team);

	bool isActive();
	void setActive(bool active);
private:
	string name;
	Team team;
	bool active;
};

#endif /* MODELS_USER_H_ */
