/*
 * MiniMapMenu.h
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#ifndef MINIMAPMENU_H_
#define MINIMAPMENU_H_

class Renderer;

class MiniMapMenu {
public:
	MiniMapMenu(int x, int y, int width, int height);
	virtual ~MiniMapMenu();
	void render(Renderer* renderer);

private:
	int x,y;
	int width,height;
};

#endif /* MINIMAPMENU_H_ */
