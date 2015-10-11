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
class MiniMapMenu;
class View;

class ScreenMenu {

public:
	ScreenMenu(int width, int height);
	virtual ~ScreenMenu();
	void render(Renderer* renderer);
	void addMiniMapSubview(View* view);

private:
	int width;
	int height;
	StatusMenu *statusMenu;
	MiniMapMenu *miniMapMenu;
};

#endif /* SCREENMENU_H_ */
