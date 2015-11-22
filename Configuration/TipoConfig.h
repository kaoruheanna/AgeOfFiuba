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
#include "../Models/Entity.h"

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
	string getImagenInteractuando();
	int getAnchoBase();
	int getAltoBase();
	int getPixelRefX();
	int getPixelRefY();
	int getFPS();
	int getDelay();
	int getAltoFrame();
	int getAnchoFrame();
	int getPixelRefXInteract();
	int getPixelRefYInteract();
	int getAltoFrameInteract();
	int getAnchoFrameInteract();
	int getMinimapPixelRefType();
	PropiedadesTipoUnidad getPropiedadesTipoUnidad();
	string getCategoria();
	list<string> getCreables();
	CostoConstruccion getCostoConstruccion();

private:
	string nombre;
	string imagen;
	string imagenDeshabilitado;
	string imagenInteractuando;
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
	int pixelRefXInteract;
	int pixelRefYInteract;
	int altoFrameInteract;
	int anchoFrameInteract;
	int minimapPixelRefType;
	PropiedadesTipoUnidad propiedadesTipoUnidad;
	string categoria;
	list<string> creables;
	CostoConstruccion costoConstruccion;

	static int getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue);
	static string getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue);
	static float getFloatAttribute(YAML::Node nodo, string attributeName, float defaultValue);

	list<string> parsearCreables(YAML::Node nodo);
};
#endif /* CONFIGURATION_TIPOCONFIG_H_ */
