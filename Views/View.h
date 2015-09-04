/*
 * View.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef VIEW_H_
#define VIEW_H_

#include "Texture.h"

class View {
public:
	View();
	virtual ~View();
	void render(SDL_Renderer* sdlRenderer );

	void setTexture(Texture *texture);

	SDL_Point origin;

private:
	Texture *texture;
};

#endif /* VIEW_H_ */
