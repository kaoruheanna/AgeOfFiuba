/*
 * TopBar.h
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#ifndef TOPBAR_H_
#define TOPBAR_H_

#include <list>
#include "../../Models/User.h"
using namespace std;

class Renderer;
class ResourceCounter;

class TopBar {
public:
	TopBar(int width, int height);
	virtual ~TopBar();
	void render(Renderer* renderer);
	void setProtagonista(User *protagonista);

private:
	int width;
	int height;
	User *protagonista;
	list<ResourceCounter*> resourcesCounters;
};

#endif /* TOPBAR_H_ */
