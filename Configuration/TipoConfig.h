/*
 * TipoConfig.h
 *
 *  Created on: Sep 14, 2015
 *      Author: nico
 */

#ifndef CONFIGURATION_TIPOCONFIG_H_
#define CONFIGURATION_TIPOCONFIG_H_

#include <stdio.h>
#include <yaml-cpp/yaml.h>
using namespace std;

enum MinimapPixelRefType {
	MinimapPixelRefTypeDefault = 0,
	MinimapPixelRefTypeCentered
};

class TipoConfig {
public:
	TipoConfig();
	TipoConfig(YAML::Node nodo);
	virtual ~TipoConfig();
	string getNombre();
	string getImagen();
	string getMiniImagen();
	string getImagenBoton();
	string getImagenDeshabilitado();
	int getAnchoBase();
	int getAltoBase();
	int getPixelRefX();
	int getPixelRefY();
	int getFPS();
	int getDelay();
	int getAltoFrame();
	int getAnchoFrame();
	int getMinimapPixelRefType();
	string getCategoria();

private:
	string nombre;
	string imagen;
	string imagenDeshabilitado;
	string miniImagen;
	string imagenBoton;
	int anchoBase;
	int altoBase;
	int pixelRefX;
	int pixelRefY;
	float fps;
	int delay;
	int altoFrame;
	int anchoFrame;
	int minimapPixelRefType;
	string categoria;

	static int getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue);
	static string getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue);
	static float getFloatAttribute(YAML::Node nodo, string attributeName, float defaultValue);
};
#endif /* CONFIGURATION_TIPOCONFIG_H_ */
