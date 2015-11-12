/*
 * ServerConectionView.h
 *
 *  Created on: Nov 8, 2015
 *      Author: nico
 */

#ifndef UTILS_SERVERCONECTIONVIEW_H_
#define UTILS_SERVERCONECTIONVIEW_H_

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class ServerConectionView {
public:
	ServerConectionView();
	virtual ~ServerConectionView();

	void showForm();
	void onServerError(string error);

	// Value getter
	string getIp();
	int getPort();
	string getUsername();
private:
	// Form values
	string ip;
	string port;
	string username;
	// View values
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;

	SDL_Texture* ipForm;
	SDL_Rect ipRect;

	SDL_Texture* portForm;
	SDL_Rect portRect;

	SDL_Texture* userForm;
	SDL_Rect userRect;

	int currentInput = -1;
	SDL_Rect submitButton;

	bool shouldQuit;
	bool showingError;

	bool init();
	void quit();
	void pullChanges();
	void connectToServer();

	void eraseCurrentInput();
	void addCurrentInput(char* newString);
	void redrawCurrentInput();
	// Click helper
	bool pointInsideRect(SDL_Point point, SDL_Rect rect);
	// Texture text helpers
	SDL_Texture* renderText(string text, SDL_Rect* textSize);
	void freeTexture(SDL_Texture* texture);
};

#endif /* UTILS_SERVERCONECTIONVIEW_H_ */
