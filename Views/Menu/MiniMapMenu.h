/*
 * MiniMapMenu.h
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#ifndef MINIMAPMENU_H_
#define MINIMAPMENU_H_

#include "MiniMapView.h"
#include <list>
using namespace std;

class Renderer;

class MiniMapMenu {
public:
	MiniMapMenu(int x, int y, int width, int height);
	virtual ~MiniMapMenu();
	void render(Renderer* renderer);
	void addView(View* view);

private:
	int x,y;
	int width,height;
	list<MiniMapView*> minimapViews;
};

#endif /* MINIMAPMENU_H_ */
