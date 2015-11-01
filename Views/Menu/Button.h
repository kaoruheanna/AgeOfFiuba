/*
 * Button.h
 *
 *  Created on: Nov 1, 2015
 *      Author: kaoru
 */

#ifndef BUTTON_H_
#define BUTTON_H_

class Renderer;
class RendererInteractionDelegate;

class Button {
public:
	Button(int x, int y, int width, int height);
	virtual ~Button();
	void render(Renderer* renderer);
	void pressed(RendererInteractionDelegate *delegate);

	int x;
	int y;
	int width;
	int height;

private:

};

#endif /* BUTTON_H_ */
