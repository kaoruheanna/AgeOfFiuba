/*
 * StatusMenu.h
 *
 *  Created on: Oct 3, 2015
 *      Author: kaoru
 */

#ifndef STATUSMENU_H_
#define STATUSMENU_H_

class Renderer;
class TextLabel;

class StatusMenu {
public:
	StatusMenu(int x, int y, int width, int height);
	virtual ~StatusMenu();
	void render(Renderer* renderer);

private:
	int x;
	int y;
	int width;
	int height;
	TextLabel *firstLabel;
	TextLabel *secondLabel;
	TextLabel *thirdLabel;
};

#endif /* STATUSMENU_H_ */
