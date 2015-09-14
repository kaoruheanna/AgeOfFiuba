/*
 * EntityView.h
 *
 *  Created on: 14/09/2015
 *      Author: fedelonghi
 */

#ifndef ENTITYVIEW_H_
#define ENTITYVIEW_H_

#include "View.h"
#include "../Models/Entity.h"

class EntityView: public View {
public:
	EntityView(std::string type);
	virtual ~EntityView();
	void setModel(Entity *model);
	void render(Renderer* renderer);

protected:
	SDL_Point getOrigin();

private:
	Entity *model;
	SDL_Point pixelRef;
};

#endif /* ENTITYVIEW_H_ */
