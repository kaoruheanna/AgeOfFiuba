/*
 * PantallaConfig.h
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#ifndef PANTALLACONFIG_H_
#define PANTALLACONFIG_H_


#include <yaml-cpp/yaml.h>


const int ALTO_PANTALLA_DEFAULT = 600;
const int ANCHO_PANTALLA_DEFAULT = 800;

class PantallaConfig {
public:
	PantallaConfig();
	PantallaConfig(YAML::Node nodo);
	~PantallaConfig();
	int getAlto();
	int getAncho();

private:
	YAML::Node nodoPantalla;
	void verificarAltoYAncho();
	bool verificarAlto();
	bool verificarAncho();

};

#endif /* PANTALLACONFIG_H_ */
