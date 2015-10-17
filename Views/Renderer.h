/*
 * Renderer.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define ISOMETRIC_DRAW true
#define ISOMETRIC_ROTATION_FACTOR 0.707

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Drawable.h"
#include <list>
#include <map>
#include "../Configuration/TipoConfig.h"
#include "Menu/ScreenMenu.h"
#include "../Models/FogOfWar.h"
using namespace std;

class View;
class EscenarioView;
class MiniEscenarioView;
class MiniView;
class TopBar;

class Renderer {
public:
	SDL_Point mainTilePosition;
	SDL_Point miniMapMainTilePosition;
	Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos);
	virtual ~Renderer();
	void close();
	void drawViews();
	void draw(int mapPositionX, int mapPositionY, Drawable* drawable); // draw Drawable
	void draw(SDL_Rect rect, SDL_Color color); // draw shape
	void drawTextureInRect(SDL_Texture *texture,SDL_Rect rect);
	SDL_Point mapToWindowPoint(SDL_Point mapPoint);
	SDL_Point windowToMapPoint(SDL_Point windowPoint);
	SDL_Point proyectedPoint(SDL_Point mapPoint, SDL_Point plano);
	bool canDraw();
	void addView(View* view);
	SDL_Renderer* getSdlRenderer();
	TTF_Font* getFont();
	void setEscenarioView(EscenarioView *escenarioView);
	void updatedEscenario();
	void setMiniEscenarioView(MiniEscenarioView *miniEscenarioView);
	void updatedMiniEscenario();
	void drawInMiniMap(int mapPositionX, int mapPositionY, Drawable* drawable);
	SDL_Point escenarioSize();
	void setFog(int ancho, int alto);
	void fogUpdate(int posicionX, int posicionY);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	list< pair<SDL_Point,Drawable*> > drawablesToPaint;
	std::map<std::string, Drawable*> drawablesByInstanceName;
	Drawable* missingImageDrawable;
	Drawable* miniMissingImageDrawable;
	ScreenMenu* screenMenu;
	TopBar* topBar;
	TTF_Font *textFont;
	EscenarioView *escenarioView;
	MiniEscenarioView *miniEscenarioView;
	FogOfWar* fog;

	int screenWidth;
	int screenHeight;

	bool initSDL();
	bool loadMedia(list<TipoConfig> tipos);
	bool loadMediaForMiniMap(list<TipoConfig>* tipos);
	bool isInsideWindow(SDL_Rect* rect);
	Drawable* getDrawableFromTipoConfig(TipoConfig tipo);
	void drawTopBar();
	void drawMenu();
	void drawEscenario();
	void drawMiniEscenario();
	int menuOriginY();
	void setDrawableForView(View* view);
	void setDrawableForMiniView(MiniView* view);
};

#endif /* RENDERER_H_ */
