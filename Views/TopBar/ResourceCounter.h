/*
 * ResourceCounter.h
 *
 *  Created on: Oct 15, 2015
 *      Author: kaoru
 */

#ifndef RESOURCECOUNTER_H_
#define RESOURCECOUNTER_H_

class TextLabel;
class Renderer;

class ResourceCounter {
public:
	ResourceCounter(int x,int y,int width,int height,char *resourceName);
	virtual ~ResourceCounter();
	void render(Renderer* renderer);

private:
	int x;
	int y;
	int width;
	int height;
	char* resourceName;
	TextLabel *label;
};

#endif /* RESOURCECOUNTER_H_ */
