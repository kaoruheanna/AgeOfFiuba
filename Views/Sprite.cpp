/*
 * Sprite.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#include "Sprite.h"
#include "../GlobalConstants.h"
#include "../Utils/Log.h"

#define STANDING_SPRITE_INDEX 	5
#define START_MOVING_INDEX 		0

const std::string TAG = "Sprite";

Sprite::Sprite(int mainTilePositionX, int mainTilePositionY, int spriteWidth, int spriteHeight, int fps)
: Drawable(mainTilePositionX, mainTilePositionY){
	this->repeatTimes = 1;
	this->fps = fps;
	this->height = spriteHeight;
	this->width = spriteWidth;
	this->animationCount = 0;
	this->framesPerAnimation = 0;
	this->isMoving = false;
	this->clipRect = {
		0, 0,
		this->width,
		this->height
	};
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

void Sprite::onTextureChange(){
	int w, h;
	SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
	this->animationCount = h / this->height;
	this->framesPerAnimation = w / this->width;
	this->repeatTimes = DELAY_MILISEC * this->fps / this->framesPerAnimation;
	Log().Get(TAG,logINFO) << "repeatTimes: "<< this->repeatTimes;
}

AnimationStatus Sprite::getAnimation(MotionDirection currentDirection, bool currentlyMoving, AnimationStatus lastStatus) {
	AnimationStatus newStatus;
	newStatus.direction = currentDirection;
	newStatus.isMoving = currentlyMoving;

	if (!currentlyMoving){
		newStatus.animationIndex = STANDING_SPRITE_INDEX * this->repeatTimes;
		return newStatus;
	}

	if (lastStatus.direction != currentDirection){
		newStatus.animationIndex = START_MOVING_INDEX;
		return newStatus;
	}

	int index = lastStatus.animationIndex;
	index++;
	if (index >= (this->framesPerAnimation * this->repeatTimes)){
		index = 0;
	}
	newStatus.animationIndex = index;
	return newStatus;
}

void Sprite::animate(AnimationStatus status){
	int currentAnimationFrame = status.animationIndex / this->repeatTimes;
	this->clipRect.x = currentAnimationFrame * this->width;
	this->clipRect.y = status.direction * this->height;
}

SDL_Rect* Sprite::getClipRect(){
	return &this->clipRect;
}
