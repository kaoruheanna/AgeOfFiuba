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
class Entity;
class Button;

class StatusMenu {
public:
	StatusMenu(int x, int y, int width, int height);
	virtual ~StatusMenu();
	void render(Renderer* renderer);
	void setMessageForFirstLabel(std::string message);
	void setMessageForSecondLabel(std::string message);
	void setStatusDataForEntity(Entity* entity);

private:
	int x;
	int y;
	int width;
	int height;
	TextLabel *firstLabel;
	TextLabel *secondLabel;
	TextLabel *thirdLabel;
	Button* entityStatusIcon;
	std::string currentEntityName;

	void setStatusIcon(Entity* entity);
	void deleteCurrentIcon();
	void setStatusBlank();
};

#endif /* STATUSMENU_H_ */
