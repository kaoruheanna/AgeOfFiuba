/*
 * ServerConectionView.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: nico
 */

#include "ServerConectionView.h"

#define TEXT_SPACING 10
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300

ServerConectionView::ServerConectionView() {
	this->ip = "127.0.0.1";
	this->port = "8080";
	this->username = "user";
	this->currentInput = 1;

	this->font = NULL;
	this->renderer = NULL;
	this->window = NULL;
	this->shouldQuit = false;

	this->ipForm = NULL;
	this->ipRect = { 0, 0, 0, 0 };
	this->portForm = NULL;
	this->portRect = { 0, 0, 0, 0 };
	this->userForm = NULL;
	this->userRect = { 0, 0, 0, 0 };

	this->showingError = false;
}

ServerConectionView::~ServerConectionView() { }

bool ServerConectionView::init() {
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		return false;
	}

	// Create window
	window = SDL_CreateWindow( "Age Of FIUBA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
	if (window == NULL) {
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if( renderer == NULL ) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	if( TTF_Init() == -1 ){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Create font
	font = TTF_OpenFont("img/arial.ttf", 16);
	if (font == NULL ) {
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	return true;
}

void ServerConectionView::quit() {
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	font = NULL;
	renderer = NULL;
	window = NULL;
}

void ServerConectionView::showForm() {
	if(!init()){
		// Some error
		return;
	}

	// Start capturing the keyboard events
	SDL_StartTextInput();

	// Initialize the form inputs
	ipForm = renderText(ip, &ipRect);
	portForm = renderText(port, &portRect);
	userForm = renderText(username, &userRect);
	// Set the views positioning
	SDL_Rect formTitleRect = { 0, TEXT_SPACING, 0, 0 };
	SDL_Texture* formTitle = renderText("Connect to the Server", &formTitleRect);
	formTitleRect.x = (WINDOW_WIDTH - formTitleRect.w) / 2;

	SDL_Rect ipLabelRect = { TEXT_SPACING, formTitleRect.y + formTitleRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* ipLabel = renderText("Server IP:", &ipLabelRect);
	ipRect.x = ipLabelRect.x + ipLabelRect.w + TEXT_SPACING;
	ipRect.y = ipLabelRect.y;

	SDL_Rect portLabelRect = { TEXT_SPACING, ipLabelRect.y + ipLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* portLabel = renderText("Server Port:", &portLabelRect);
	portRect.x = portLabelRect.x + portLabelRect.w + TEXT_SPACING;
	portRect.y = portLabelRect.y;

	SDL_Rect userLabelRect = { TEXT_SPACING, portLabelRect.y + portLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* userLabel = renderText("User Name:", &userLabelRect);
	userRect.x = userLabelRect.x + userLabelRect.w + TEXT_SPACING;
	userRect.y = userLabelRect.y;

	SDL_Rect formButtonRect = { 0, userLabelRect.y + userLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* formButton = renderText("Connect", &formButtonRect);
	formButtonRect.x = (WINDOW_WIDTH - formButtonRect.w) / 2;
	submitButton = {
		formButtonRect.x - TEXT_SPACING / 2,
		formButtonRect.y - TEXT_SPACING / 2,
		formButtonRect.w + TEXT_SPACING,
		formButtonRect.h + TEXT_SPACING
	};

	while(!shouldQuit){
		// Update texts
		pullChanges();

		// Clear screen
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(renderer);

		// Draw
		SDL_RenderCopy(renderer, formTitle, NULL, &formTitleRect);

		SDL_RenderCopy(renderer, ipLabel, NULL, &ipLabelRect);
		SDL_RenderCopy(renderer, ipForm, NULL, &ipRect);

		SDL_RenderCopy(renderer, portLabel, NULL, &portLabelRect);
		SDL_RenderCopy(renderer, portForm, NULL, &portRect);

		SDL_RenderCopy(renderer, userLabel, NULL, &userLabelRect);
		SDL_RenderCopy(renderer, userForm, NULL, &userRect);

		SDL_RenderCopy(renderer, formButton, NULL, &formButtonRect);
		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect(renderer, &submitButton);

		// Update screen
		SDL_RenderPresent(renderer);

		SDL_Delay( 50 );
	}

	// Free memory
	freeTexture(ipForm);
	freeTexture(portForm);
	freeTexture(userForm);

	freeTexture(formTitle);
	freeTexture(formButton);

	freeTexture(ipLabel);
	freeTexture(portLabel);
	freeTexture(userLabel);

	// Stop capturing the keyboard events
	SDL_StopTextInput();

	quit();
}

void ServerConectionView::pullChanges() {
	bool updateString = false;
	SDL_Rect ipInput = { 0, ipRect.y, WINDOW_WIDTH, ipRect.h };
	SDL_Rect portInput = { 0, portRect.y, WINDOW_WIDTH, portRect.h };
	SDL_Rect userInput = { 0, userRect.y, WINDOW_WIDTH, userRect.h };
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) {

		if( e.type == SDL_QUIT ) {
			shouldQuit = true;
		}
		if(!showingError){
			// Detect what is clicking
			if (e.type == SDL_MOUSEBUTTONDOWN){
				// Get mouse position
				SDL_Point clickPoint = { 0, 0 };
				SDL_GetMouseState(&clickPoint.x, &clickPoint.y);

				if(pointInsideRect(clickPoint, submitButton)){
					connectToServer();
				} else if(pointInsideRect(clickPoint, ipInput)){
					this->currentInput = 1;
				} else if(pointInsideRect(clickPoint, portInput)){
					this->currentInput = 2;
				} else if(pointInsideRect(clickPoint, userInput)){
					this->currentInput = 3;
				}
			}

			if( e.type == SDL_KEYDOWN ){
				if( e.key.keysym.sym == SDLK_BACKSPACE && ip.length() > 0 ){
					this->eraseCurrentInput();
					updateString = true;
				}
				// Normal form helpers
				if(e.key.keysym.sym == SDLK_TAB){
					switch(this->currentInput){
						case 1:
						case 2:
							this->currentInput ++;
							break;
						default:
							this->currentInput = 1;
					}
				}
				if(e.key.keysym.sym == SDLK_RETURN){
					this->connectToServer();
				}
			}

			if( e.type == SDL_TEXTINPUT ){
				this->addCurrentInput(e.text.text);
				updateString = true;
			}
		}
	}

	if(updateString){
		this->redrawCurrentInput();
	}
}

void ServerConectionView::eraseCurrentInput() {
	switch(currentInput){
		case 1:
			if(ip.length() > 0){
				ip.erase(ip.length() - 1, 1);
			}
			break;
		case 2:
			if(port.length() > 0){
				port.erase(port.length() - 1, 1);
			}
			break;
		case 3:
			if(username.length() > 0){
				username.erase(username.length() - 1, 1);
			}
			break;
		default:
			break;
	}
}

void ServerConectionView::addCurrentInput(char* newString) {
	switch(currentInput){
		case 1:
			ip += newString;
			break;
		case 2:
			port += newString;
			break;
		case 3:
			username += newString;
			break;
		default:
			break;
	}
}

void ServerConectionView::redrawCurrentInput() {
	switch(currentInput){
		case 1:
			freeTexture(ipForm);
			ipForm = renderText(ip, &ipRect);
			break;
		case 2:
			freeTexture(portForm);
			portForm = renderText(port, &portRect);
			break;
		case 3:
			freeTexture(userForm);
			userForm = renderText(username, &userRect);
			break;
		default:
			break;
	}
}

void ServerConectionView::connectToServer() {
	// Only close the view :D
	this->shouldQuit = true;
}

// Click helper
bool ServerConectionView::pointInsideRect(SDL_Point point, SDL_Rect rect){
	return (rect.x <= point.x &&
			rect.x + rect.w >= point.x &&
			rect.y <= point.y &&
			rect.y + rect.h >= point.y);
}

// Texture text helpers
void ServerConectionView::freeTexture(SDL_Texture* texture) {
	if(texture != NULL){
		SDL_DestroyTexture(texture);
	}
}

SDL_Texture* ServerConectionView::renderText(string text, SDL_Rect* textSize) {
	SDL_Color textColor = {0,0,0};
	SDL_Surface* surfaceText = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if(surfaceText == NULL) {
		// Error => sin surface
		return NULL;
	}
	textSize->w = surfaceText->w;
	textSize->h = surfaceText->h;

	SDL_Texture* renderedText = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface( surfaceText );
	if (renderedText == NULL){
		// Error => sin texto
		textSize->w = 0;
		textSize->h = 0;
	}
	return renderedText;
}

// Value getter
string ServerConectionView::getIp() {
	return this->ip;
}

int ServerConectionView::getPort() {
	return strtoimax(port.c_str(), NULL, 10);
}

string ServerConectionView::getUsername() {
	return this->username;
}

// Error View
void ServerConectionView::onServerError(string error){
	this->shouldQuit = false;
	this->showingError = true;

	if(!init()){
		// Some error
		return;
	}

	SDL_Rect errorRect = { 0, 0, 0, 0 };
	SDL_Texture* errorText = renderText(error, &errorRect);

	while(!shouldQuit){
		// Wait for the end
		pullChanges();

		// Clear screen
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(renderer);

		// Draw
		SDL_RenderCopy(renderer, errorText, NULL, &errorRect);

		// Update screen
		SDL_RenderPresent(renderer);

		SDL_Delay( 50 );
	}

	freeTexture(errorText);

	quit();
}
