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

class TipoConfig {
public:
	TipoConfig();
	TipoConfig(YAML::Node nodo);
	virtual ~TipoConfig();
	string getNombre();
	string getImagen();
	int getAnchoBase();
	int getAltoBase();
	int getPixelRefX();
	int getPixelRefY();
	int getFPS();
	int getDelay();
	int getAltoFrame();
	int getAnchoFrame();

private:
	string nombre;
	string imagen;
	int anchoBase;
	int altoBase;
	int pixelRefX;
	int pixelRefY;
	int fps;
	int delay;
	int altoFrame;
	int anchoFrame;

	static int getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue);
	static string getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue);
};
#endif /* CONFIGURATION_TIPOCONFIG_H_ */
