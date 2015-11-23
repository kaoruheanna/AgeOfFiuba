/*
 * ActionsMenu.h
 *
 *  Created on: Oct 29, 2015
 *      Author: kaoru
 */

#ifndef ACTIONSMENU_H_
#define ACTIONSMENU_H_

#include <list>
#include <string>
using namespace std;

class Renderer;
class RendererInteractionDelegate;
class Button;
class Entity;
class User;
class TextLabel;

class ActionsMenu {
public:
	ActionsMenu(int x, int y, int width, int height);
	virtual ~ActionsMenu();
	void render(Renderer* renderer,Entity *selectedEntity);
	void clickEvent(int x, int y, RendererInteractionDelegate *delegate);
	void setUser(User *user);
	void showEntityCost(int x, int y);

private:
	int x;
	int y;
	int width;
	int height;
	list<Button*> buttons;
	int entityID;
	User *user;
	TextLabel* foodCostTextLabel;
	TextLabel* woodCostTextLabel;
	TextLabel* stoneCostTextLabel;
	TextLabel* goldCostTextLabel;
	TextLabel* buttonNameLabel;

	bool isPixelInButton(int x, int y, Button *button);
	void setButtonsForSelectedEntity(Entity *selectedEntity);
	void deleteButtons();
	void updateButtons();
	void setLabelsBlank();
	void renderLabels(Renderer* renderer);
	std::string convertIntToString(int number);
};

#endif /* ACTIONSMENU_H_ */
