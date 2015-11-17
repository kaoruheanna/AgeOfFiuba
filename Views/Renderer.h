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
#include "../Models/User.h"
using namespace std;

class View;
class EscenarioView;
class MiniEscenarioView;
class MiniView;
class TopBar;
class Cartel;

class RendererInteractionDelegate {
public:
	RendererInteractionDelegate(){};
	virtual ~RendererInteractionDelegate(){};
	virtual void leftClickEnEscenario(int x, int y){};
	virtual void rightClickEnEscenario(int x, int y){};
	virtual void leftMouseUp(int x, int y, int w, int h){};
	virtual void createEntityButtonPressed(string entityName){};
};

struct DrawableWithState {
	Drawable* drawable;
	SDL_Rect rect;
	bool hasRect;
};

class Renderer {
public:
	SDL_Point mainTilePosition;
	SDL_Point miniMapMainTilePosition;
	Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos);
	virtual ~Renderer();
	void close();
	void drawViews();
	void draw(int mapPositionX, int mapPositionY, Drawable* drawable, bool admiteNublado); // draw Drawable
	void draw(SDL_Rect rect, SDL_Color color); // draw shape
	void drawTextureInRect(SDL_Texture *texture,SDL_Rect rect);
	void drawActionButtonWithNameInRect(string name, SDL_Rect rect);
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
	void drawInMiniMap(int mapPositionX, int mapPositionY, Drawable* drawable, bool admiteNublado);
	SDL_Point escenarioSize();
	void setFog(int ancho, int alto);
	void fogUpdate(list<Entity*> entidades, Team equipo);
	FogOfWar* getFog();
	void setProtagonista(User *protagonista);
	void setMessagesInMenu(std::string firstMessage, std::string secondMessage);
	void setSelectedTilesCoordinates(bool selected,std::list<pair<SDL_Point,SDL_Point>> tiles, list<Entity*> entidad);
	void setCartel(string message);
	void hideCartel();
	void clickEvent(int x, int y, bool leftClick, RendererInteractionDelegate *delegate);
	void dragLeftClickEvent(int xi, int yi, int xf, int yf);
	void leftMouseUpEvent(RendererInteractionDelegate *delegate, int x, int y);
	Entity* selectedEntity;

	list <Entity*> selectedEntities;

	bool isPixelInEscenario(int x, int y);
	bool isPixelInMenu(int x, int y);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	list< pair<SDL_Point,DrawableWithState> > drawablesToPaint;
	std::map<std::string, Drawable*> drawablesByInstanceName;
	Drawable* missingImageDrawable;
	Drawable* miniMissingImageDrawable;
	ScreenMenu* screenMenu;
	TopBar* topBar;
	Cartel* cartel;
	TTF_Font *textFont;
	EscenarioView *escenarioView;
	MiniEscenarioView *miniEscenarioView;
	FogOfWar* fog;
	std::pair<SDL_Point,SDL_Point> selectedTilesCoordinates;
	bool hasSelectedTiles;
	SDL_Rect topBarRect;
	SDL_Rect escenarioRect;
	SDL_Rect menuRect;
	SDL_Rect minimapRect;
	SDL_Rect selectionArea;
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
	void drawCartelIfShould();
	void drawMiniEscenario();
	void drawSelectionRect();
	int menuOriginY();
	void setDrawableForView(View* view);
	void setDrawableForMiniView(MiniView* view);
	void initRects();
	bool isPixelInRect(int x, int y, SDL_Rect rect);
	bool sonTilesIguales(int tileX, int tileY, int selectedTileX, int selectedTileY);
	void setearColor(Drawable* drawable);
};

#endif /* RENDERER_H_ */
