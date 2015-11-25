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
#include <map>
#include <list>
#include "../Red/Serializable.h"

using namespace std;

class User : public Serializable{
public:
	User(string name);
	virtual ~User();

	void update(User* newData);

	// Getter / Setter
	string getName();

	Team getTeam();
	void setTeam(Team team);

	bool isActive();
	void setActive(bool active);
	bool puedePagar(CostoConstruccion costo);
	void pagarCosto(CostoConstruccion costo);

	bool estaJugando();
	bool yaPerdio();
	bool yaGano();

	bool perdio;
	bool gano;


	int comida;
	int piedra;
	int madera;
	int oro;

	// Resource methods

	void addResourceToCollect(string resourceName);
	void didCollectResource(string resourceName);
	list<string> getResourcesNames();
	int getValueForResource(string resourceName);
	void setResourceValues(int comida,int madera,int piedra,int oro);


	// Serializable methods
	User();

	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	string name;
	Team team;
	bool active;
	list<string> resourceNames;
	static const int INITIAL_RESOURCES = 500;
};

#endif /* MODELS_USER_H_ */
