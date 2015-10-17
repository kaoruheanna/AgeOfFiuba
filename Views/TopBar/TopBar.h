/*
 * TopBar.h
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#ifndef TOPBAR_H_
#define TOPBAR_H_

class Renderer;
class ResourceCounter;

class TopBar {
public:
	TopBar(int width, int height);
	virtual ~TopBar();
	void render(Renderer* renderer);

private:
	int width;
	int height;
	ResourceCounter *firstResourceCounter;
	ResourceCounter *secondResourceCounter;
	ResourceCounter *thirdResourceCounter;
};

#endif /* TOPBAR_H_ */
