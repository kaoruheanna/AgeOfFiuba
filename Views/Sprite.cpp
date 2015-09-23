/*
 * Sprite.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#include "Sprite.h"
#include "../GlobalConstants.h"
#include "../Utils/Log.h"

#define START_MOVING_INDEX 		0

const std::string TAG = "Sprite";

Sprite::Sprite(int mainTilePositionX, int mainTilePositionY, int spriteWidth, int spriteHeight, float fps, float delay)
: Drawable(mainTilePositionX, mainTilePositionY){

	this->fps = fps;
	this->delay = delay;
	this->framesPerAnimation = 0;
	this->height = spriteHeight;
	this->width = spriteWidth;
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

	this->framesPerAnimation = w / this->width;

	// Calcula cuanto tiempo se queda en un frame la animacion
	float framesPerDraw = this->fps * DELAY_MILISEC / 1000;
	float currentFrames = 0; // Frames actuales para una llamada de draw

	// Simula llamadas de draw hasta que llega al frame final de la animacion
	while(currentFrames < this->framesPerAnimation){
		this->frameIndexes.push_back(floor(currentFrames));
		currentFrames += framesPerDraw;
	}

	int delayFrames = ((this->delay * 1000) / DELAY_MILISEC);

	for (int i = 0; i < delayFrames; i++){
		this->frameIndexes.push_back(START_MOVING_INDEX);
	}
}

AnimationStatus Sprite::getAnimation(MotionDirection currentDirection, bool currentlyMoving, AnimationStatus lastStatus) {
	AnimationStatus newStatus;
	newStatus.direction = currentDirection;
	newStatus.isMoving = currentlyMoving;
	newStatus.animationIndex = 0;

	// si no se esta moviendo no importa el animationIndex
	if (!currentlyMoving){
		return newStatus;
	}

	if (lastStatus.direction != currentDirection){
		newStatus.animationIndex = 0;
		return newStatus;
	}

	int index = lastStatus.animationIndex;
	index++;
	if (index >= (int)this->frameIndexes.size()){
		index = 0;
	}
	newStatus.animationIndex = index;
	return newStatus;
}

void Sprite::animate(AnimationStatus status){
	//por convencion, si no se esta parado usamos la ultima
	int frameIndex = (status.isMoving) ? this->frameIndexes[status.animationIndex] : (this->framesPerAnimation - 1) ;
	this->clipRect.x = frameIndex * this->width;
	this->clipRect.y = status.direction * this->height;
}

SDL_Rect* Sprite::getClipRect(){
	return &this->clipRect;
}
