/*
 * GlobalConstants.h
 *
 *  Created on: Sep 17, 2015
 *      Author: kaoru
 */

#ifndef GLOBALCONSTANTS_H_
#define GLOBALCONSTANTS_H_

const int DELAY_MILISEC = 20; //50 frames por segundo
const int PROTAGONISTA_SPEED = 5;
const int SCROLL_SPEED = 20;
const int MENU_HEIGHT = 130;
const int MENU_SPACING = 10;

//TILES
const std::string TILE_DEFAULT_NAME = "tileDefault";
const std::string TILE_DEFAULT_PATH = "img/grass1n.png";
const std::string MINI_TILE_DEFAULT_NAME = "miniTileDefault";
const std::string MINI_TILE_DEFAULT_PATH = "img/tile_green.png";
const std::string MISSING_IMAGE_PATH = "img/missingImage.png";
const std::string MINI_MISSING_IMAGE_PATH = "img/tile_purple.png";

const int TILE_WIDTH_PIXELS = 128;
const int TILE_HEIGHT_PIXELS = 64;

const std::string CONFIG_DEFAULT = "yaml-files/configuracion.yaml";
const std::string CONFIG_CUSTOM = "yaml-files/_configuracionMapa.yaml";

const int  FOG_VISIBLE = 255;
const int  FOG_VISITED = 160;
const bool FOG_ENABLED = true;

#endif /* GLOBALCONSTANTS_H_ */
