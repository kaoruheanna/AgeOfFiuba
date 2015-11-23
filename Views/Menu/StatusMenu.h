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
	TextLabel *entityNameLabel;
	TextLabel *entityTeamLabel;
	TextLabel *remainingLifeLabel;
	TextLabel *attackFeatureLabel;
	TextLabel *defenseFeatureLabel;
	TextLabel *constructionProgressLabel;
	Button* entityStatusIcon;
	std::string currentEntityName;
	Entity* entityClicked;

	void setStatusIcon();
	void deleteCurrentIcon();
	void setStatusBlank();
	std::string convertIntToString(int number);
	void setSpecialFeatures();
	void setLabels();
	void actualizarVida();
	void actualizarProgreso();
};

#endif /* STATUSMENU_H_ */
