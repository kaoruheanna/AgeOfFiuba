/*
 * MiniMapView.h
 *
 *  Created on: Oct 6, 2015
 *      Author: kaoru
 */

#ifndef MINIMAPVIEW_H_
#define MINIMAPVIEW_H_

#include "MiniView.h"

class View;
class Renderer;
class Map;

class MiniMapView: public MiniView {
public:
	MiniMapView(std::string type);
	virtual ~MiniMapView();
	void setModel(Map *model);
	virtual void render(Renderer* renderer);

private:
	Map *model;
};

#endif /* MINIMAPVIEW_H_ */
