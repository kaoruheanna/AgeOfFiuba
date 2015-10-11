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
//	void addView(View* view);
	MapView* getMapView();

private:
	MapView *mapView;
	list<View*> views;
};

#endif /* ESCENARIOVIEW_H_ */
