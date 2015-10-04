/*
 * ScreenMenu.h
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#ifndef SCREENMENU_H_
#define SCREENMENU_H_

class Renderer;
class StatusMenu;

class ScreenMenu {

public:
	ScreenMenu(int x, int y, int width, int height);
	virtual ~ScreenMenu();
	void render(Renderer* renderer);
	int getY();

private:
	int x;
	int y;
	int width;
	int height;
	StatusMenu *statusMenu;
};

#endif /* SCREENMENU_H_ */
