/*
 * GlobalConstants.h
 *
 *  Created on: Sep 17, 2015
 *      Author: kaoru
 */

#ifndef GLOBALCONSTANTS_H_
#define GLOBALCONSTANTS_H_

#include <string>

const int DELAY_MILISEC = 20; //50 frames por segundo
const int PROTAGONISTA_SPEED = 5;
const int SCROLL_SPEED = 20;
const int MENU_HEIGHT = 130;
const int MENU_SPACING = 10;
const int TOP_BAR_HEIGHT = 30;
const int CARTEL_WIDTH = 300;
const int CARTEL_HEIGHT = 100;

// Configuracion para que aparezcan nuevos resources
const bool GENERATE_RESOURCES = false;

//TILES
const std::string TILE_DEFAULT_NAME = "tileDefault";
const std::string TILE_DEFAULT_PATH = "img/grass1_32.png";
const std::string MINI_TILE_DEFAULT_NAME = "miniTileDefault";
const std::string MINI_TILE_DEFAULT_PATH = "img/tile_green.png";
const std::string MISSING_IMAGE_PATH = "img/missingImage.png";
const std::string MINI_MISSING_IMAGE_PATH = "img/tile_purple.png";

const int TILE_WIDTH_PIXELS = 64;
const int TILE_HEIGHT_PIXELS = 32;
const int TILE_SIZE = 32;

const std::string CONFIG_DEFAULT = "yaml-files/configuracion.yaml";
const std::string CONFIG_CUSTOM = "yaml-files/configuracion.yaml";
const std::string CONFIG_CLIENT = "yaml-files/configuracion_cliente.yaml";

//equipos
const std::string NOMBRE_EQUIPO_NEUTRAL = "NEUTRAL";
const std::string NOMBRE_EQUIPO_RED = "RED";
const std::string NOMBRE_EQUIPO_BLUE = "BLUE";
const std::string NOMBRE_EQUIPO_GREEN = "GREEN";
const std::string NOMBRE_EQUIPO_YELLOW = "YELLOW";

const int  FOG_VISIBLE = 255;
const int  FOG_VISITED = 128;

typedef std::pair <int,int> TileCoordinate;
typedef std::pair <int,int> LogicPosition;

#endif /* GLOBALCONSTANTS_H_ */
