/*
 * MiniEscenarioView.h
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#ifndef MINIESCENARIOVIEW_H_
#define MINIESCENARIOVIEW_H_

class MiniMapView;
class Renderer;

class MiniEscenarioView {
public:
	MiniEscenarioView(MiniMapView *miniMapView);
	virtual ~MiniEscenarioView();
	MiniMapView* getMiniMapView();
	void render(Renderer* renderer);

private:
	MiniMapView *miniMapView;
};

#endif /* MINIESCENARIOVIEW_H_ */
