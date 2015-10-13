/*
 * MiniEscenarioView.h
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#ifndef MINIESCENARIOVIEW_H_
#define MINIESCENARIOVIEW_H_

#include <list>
using namespace std;

class MiniMapView;
class Renderer;
class MiniView;

class MiniEscenarioView {
public:
	MiniEscenarioView(MiniMapView *miniMapView);
	virtual ~MiniEscenarioView();
	MiniMapView* getMiniMapView();
	void render(Renderer* renderer);
	void addEntityMiniView(MiniView* miniview);
	list<MiniView*>* getEntitiesMiniView();

private:
	MiniMapView *miniMapView;
	list<MiniView*> entitiesMiniViews;
};

#endif /* MINIESCENARIOVIEW_H_ */
