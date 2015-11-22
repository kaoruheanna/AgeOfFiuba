/*
 * EscenarioView.h
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#ifndef ESCENARIOVIEW_H_
#define ESCENARIOVIEW_H_

#include <list>
using namespace std;

class Renderer;
class MapView;
class View;

class EscenarioView {
public:
	EscenarioView(MapView *mapView);
	virtual ~EscenarioView();
	void render(Renderer* renderer);
	void addEntityView(View* view);
	MapView* getMapView();
	list<View*>* getEntitiesView();
	void removeEntityViewForId(int id);
	bool hasViewForEntityWithId(int id);

private:
	MapView *mapView;
	list<View*> entitiesViews;
};

#endif /* ESCENARIOVIEW_H_ */
