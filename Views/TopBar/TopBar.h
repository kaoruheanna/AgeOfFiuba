/*
 * TopBar.h
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#ifndef TOPBAR_H_
#define TOPBAR_H_

#include <list>
using namespace std;

class Renderer;
class ResourceCounter;
class MobileModel;

class TopBar {
public:
	TopBar(int width, int height);
	virtual ~TopBar();
	void render(Renderer* renderer);
	void setProtagonista(MobileModel *protagonista);

private:
	int width;
	int height;
	MobileModel *protagonista;
	list<ResourceCounter*> resourcesCounters;
};

#endif /* TOPBAR_H_ */
