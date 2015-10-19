/*
 * ResourceCounter.h
 *
 *  Created on: Oct 15, 2015
 *      Author: kaoru
 */

#ifndef RESOURCECOUNTER_H_
#define RESOURCECOUNTER_H_

#include <string>
using namespace std;

class TextLabel;
class Renderer;

class ResourceCounter {
public:
	ResourceCounter(int x,int y,int width,int height,string resourceName);
	virtual ~ResourceCounter();
	void render(Renderer* renderer);
	string getResourceName();
	void setResourceValue(int value);

private:
	int x;
	int y;
	int width;
	int height;
	string resourceName;
	TextLabel *label;
};

#endif /* RESOURCECOUNTER_H_ */
