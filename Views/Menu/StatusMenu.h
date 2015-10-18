/*
 * StatusMenu.h
 *
 *  Created on: Oct 3, 2015
 *      Author: kaoru
 */

#ifndef STATUSMENU_H_
#define STATUSMENU_H_

#include <string>

class Renderer;
class TextLabel;

class StatusMenu {
public:
	StatusMenu(int x, int y, int width, int height);
	virtual ~StatusMenu();
	void render(Renderer* renderer);
	void setMessageForFirstLabel(std::string message);
	void setMessageForSecondLabel(std::string message);

private:
	int x;
	int y;
	int width;
	int height;
	TextLabel *firstLabel;
	TextLabel *secondLabel;
};

#endif /* STATUSMENU_H_ */
