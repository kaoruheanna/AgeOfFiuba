/*
 * ActionsMenu.h
 *
 *  Created on: Oct 29, 2015
 *      Author: kaoru
 */

#ifndef ACTIONSMENU_H_
#define ACTIONSMENU_H_

class Renderer;

class ActionsMenu {
public:
	ActionsMenu(int x, int y, int width, int height);
	virtual ~ActionsMenu();
	void render(Renderer* renderer);

private:
	int x;
	int y;
	int width;
	int height;
};

#endif /* ACTIONSMENU_H_ */
