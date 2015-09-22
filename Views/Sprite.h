/*
 * Sprite.h
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#ifndef VIEWS_SPRITE_H_
#define VIEWS_SPRITE_H_

#include <vector>
#include "Drawable.h"
#include "MobileView.h"
using namespace std;

class Sprite: public Drawable {
public:
	Sprite(int mainTilePositionX, int mainTilePositionY, int spriteWidth, int spriteHeight, float fps, float delay);
	virtual ~Sprite();
	SDL_Rect* getClipRect();
	AnimationStatus getAnimation(MotionDirection currentDirection, bool currentlyMoving, AnimationStatus lastStatus);
	void animate(AnimationStatus status);

protected:
	void onTextureChange();

private:
	float fps;
	float delay;
	SDL_Rect clipRect;
	vector<int> frameIndexes;
	int framesPerAnimation;
};

#endif /* VIEWS_SPRITE_H_ */
