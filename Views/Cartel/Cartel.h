/*
 * Cartel.h
 *
 *  Created on: Oct 20, 2015
 *      Author: kaoru
 */

#ifndef CARTEL_H_
#define CARTEL_H_

#include <string>
using namespace std;

class Renderer;
class TextLabel;

class Cartel {
public:
	Cartel(int x, int y);
	virtual ~Cartel();
	void render(Renderer* renderer);
	void setMessage(string message);

private:
	int x;
	int y;
	TextLabel *label;
};

#endif /* CARTEL_H_ */
