/*
 * TextLabel.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#include "TextLabel.h"
#include "../Renderer.h"
#include "../../Utils/Log.h"

const std::string TAG = "TextLabel";

TextLabel::TextLabel(int x, int y) {
	this->origin = {x,y};
	this->width = 0;
	this->height = 0;
	this->texture = NULL;
	this->message = "";
	this->setTextLabelColor( 0, 0, 0);
}

TextLabel::~TextLabel() {
	this->freeTexture();
}

void TextLabel::render(Renderer *renderer){
	if (this->message == ""){
		return;
	}

	if (this->texture == NULL){
		this->loadTexture(renderer->getSdlRenderer(),renderer->getFont());
	}
	SDL_Rect rect = {this->origin.x,this->origin.y,this->width,this->height};
	renderer->drawTextureInRect(this->texture,rect);
}

void TextLabel::loadTexture(SDL_Renderer *sdlRenderer,TTF_Font *font){
	this->freeTexture();
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, this->message.c_str(), this->textColor);
	if(surfaceMessage == NULL) {
		Log().Get(TAG,logERROR) << "No se pudo cargar la surface: "<<TTF_GetError();
		return;
	}

	this->width = surfaceMessage->w;
	this->height = surfaceMessage->h;

	this->texture = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage);
	SDL_FreeSurface( surfaceMessage );
	if (this->texture == NULL){
		Log().Get(TAG,logERROR) << "No se pudo crear la texture: "<<SDL_GetError();
		return;
	}
}

void TextLabel::setTextLabelColor(Uint8 red, Uint8 green, Uint8 blue){
	this->textColor = {red,green,blue};
	this->freeTexture();
}

void TextLabel::setMessage(string newMessage) {
	if (this->message == newMessage){
		return;
	}
	this->message = newMessage;
	this->freeTexture();
}

void TextLabel::freeTexture() {
	if (this->texture){
		SDL_DestroyTexture(this->texture);
		this->texture = NULL;
	}
}
