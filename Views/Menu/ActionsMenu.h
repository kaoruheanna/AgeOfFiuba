/*
 * ActionsMenu.h
 *
 *  Created on: Oct 29, 2015
 *      Author: kaoru
 */

#ifndef ACTIONSMENU_H_
#define ACTIONSMENU_H_

class Renderer;
class RendererInteractionDelegate;
class Button;

class ActionsMenu {
public:
	ActionsMenu(int x, int y, int width, int height);
	virtual ~ActionsMenu();
	void render(Renderer* renderer);
	void clickEvent(int x, int y, RendererInteractionDelegate *delegate);

private:
	int x;
	int y;
	int width;
	int height;
	Button *button;

	bool isPixelInButton(int x, int y, Button *button);
};

#endif /* ACTIONSMENU_H_ */
