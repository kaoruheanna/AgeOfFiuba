/*
 * Button.h
 *
 *  Created on: Nov 1, 2015
 *      Author: kaoru
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <string>
using namespace std;

class Renderer;
class RendererInteractionDelegate;

class Button {
public:
	Button(int x, int y, int width, int height);
	virtual ~Button();
	void render(Renderer* renderer);
	void pressed(RendererInteractionDelegate *delegate);
	void setEntityName(string name);
	string getEntityName();

	int x;
	int y;
	int width;
	int height;
	bool enabled;

private:
	string entityName;

};

#endif /* BUTTON_H_ */
