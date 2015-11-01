/*
 * ScreenMenu.h
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#ifndef SCREENMENU_H_
#define SCREENMENU_H_

#include <string>

class Renderer;
class RendererInteractionDelegate;
class StatusMenu;
class View;
class ActionsMenu;

class ScreenMenu {

public:
	ScreenMenu(int width, int height);
	virtual ~ScreenMenu();
	void render(Renderer* renderer);
	void setMessages(std::string firstMessage, std::string secondMessage);
	void clickEvent(int x, int y, RendererInteractionDelegate *delegate);

private:
	int width;
	int height;
	StatusMenu *statusMenu;
	ActionsMenu *actionsMenu;
};

#endif /* SCREENMENU_H_ */
