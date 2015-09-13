/*
 * MapView.h
 *
 *  Created on: 12/09/2015
 *      Author: fedelonghi
 */

#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include "View.h"
#include "../Models/Map.h"


class MapView: public View{
public:
	MapView(std::string type);
	virtual ~MapView();
	void setModel(Map *model);
	void render(Renderer* renderer );

protected:
	SDL_Point getOrigin();

private:
	Map *model;
};




#endif /* MAPVIEW_H_ */
