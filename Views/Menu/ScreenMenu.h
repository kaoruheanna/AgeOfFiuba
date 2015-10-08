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
class View;

class ScreenMenu {

public:
	ScreenMenu(int width, int height);
	virtual ~ScreenMenu();
	void render(Renderer* renderer);

private:
	int width;
	int height;
	StatusMenu *statusMenu;
};

#endif /* SCREENMENU_H_ */
