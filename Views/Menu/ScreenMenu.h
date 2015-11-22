/*
 * ScreenMenu.h
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#ifndef SCREENMENU_H_
#define SCREENMENU_H_

#include <string>
#include <list>

class Renderer;
class RendererInteractionDelegate;
class StatusMenu;
class View;
class ActionsMenu;
class Entity;

class ScreenMenu {

public:
	ScreenMenu(int width, int height);
	virtual ~ScreenMenu();
	//void render(Renderer* renderer,Entity *selectedEntity);
	void renderMenu(Renderer* renderer);
	void renderMenuForEntity(Renderer* renderer,std::list<Entity *>selectedEntity);
	void clickEvent(int x, int y, RendererInteractionDelegate *delegate);
	void setStatusForEntity(Entity* entity);

private:
	int width;
	int height;
	StatusMenu *statusMenu;
	ActionsMenu *actionsMenu;
};

#endif /* SCREENMENU_H_ */
