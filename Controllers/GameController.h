/*
 * GameController.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

class GameController {
public:
	GameController();
	virtual ~GameController();
	void play();

private:
	bool shouldQuit;

	void pollEvents();
};

#endif /* GAMECONTROLLER_H_ */
