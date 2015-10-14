/*
 * Serializable.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: nico
 */

#include "Serializable.h"

#include <unistd.h>
#include "../Utils/Log.h"

const string tag = "Serializable";

Serializable::Serializable() { }

Serializable::~Serializable() { }

int Serializable::getTotalBlockCount() {
	return -1;
}
int Serializable::getBlockSizeFromIndex(int currentIndex) {
	return -1;
}
void Serializable::getBlockFromIndex(int currentIndex, void* buffer) {
	return;
}
void Serializable::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	return;
}

// Metodos para recibir y enviar serializables
int enviarSerializable(int sd, Serializable* serializable) {
	Log().Get(tag, logINFO) << "Serializando...";
	int totalEnviado = 0;
	int bloques = serializable->getTotalBlockCount();
	int bloqueActual = 0;
	Log().Get(tag, logINFO) << "Enviando numero de bloques: "<< bloques;
	int enviado = write(sd, &bloques, sizeof(int));
	while(bloqueActual < bloques && enviado > -1){
		Log().Get(tag, logINFO) << "Serializando bloque: "<< bloqueActual;
		int tamanio = serializable->getBlockSizeFromIndex(bloqueActual);
		Log().Get(tag, logINFO) << "Enviando tamanio bloque: "<< tamanio;
		enviado = write(sd, &tamanio, sizeof(int));
		if(enviado > -1){
			void* buffer = malloc(tamanio);
			Log().Get(tag, logINFO) << "Enviando bloque...";
			serializable->getBlockFromIndex(bloqueActual, buffer);
			Log().Get(tag, logINFO) << "Bloque enviado";
			enviado = write(sd, buffer, tamanio);
			free(buffer);
			totalEnviado += enviado;
			bloqueActual++;
		}
	}
	if(enviado < 0){
		Log().Get(tag, logERROR) << "Error serializando: "<< enviado;
		return enviado;
	}
	Log().Get(tag, logINFO) << "Serializado";
	return totalEnviado;
}

void* recibirInfo(int sd, int tamanio) {
	Log().Get(tag, logINFO) << "Leyendo info de tamanio " << tamanio;
	char* infoBuffer = (char*) malloc(tamanio);
	char* bufferActual = infoBuffer;
	int faltaLeer = tamanio;
	int leido = 0;
	do {
		Log().Get(tag, logINFO) << "Esperando info de tamanio " << faltaLeer;
		leido = read(sd, bufferActual, faltaLeer);
		Log().Get(tag, logINFO) << "Leido " << leido;
		faltaLeer -= leido;
		bufferActual = bufferActual + leido;
	} while((leido > 0) && (faltaLeer > 0));
	if(leido < 0){
		Log().Get(tag, logINFO) << "Error leyendo info " << leido;
		free(infoBuffer);
		infoBuffer = NULL;
	}
	return (void*)infoBuffer;
}

int recibirSerializable(int sd, Serializable* serializable) {
	Log().Get(tag, logINFO) << "Recibiendo serializable";
	void* lecturaActual = recibirInfo(sd, sizeof(int)); // Obtener tamanio en bloques
	if(lecturaActual == NULL){
		return -1; // ERR: sin tamanio
	}
	int bloquesTotales = *(int*)lecturaActual;
	Log().Get(tag, logINFO) << "Bloques del serializable: " << bloquesTotales;
	free(lecturaActual);
	int bloqueActual = 0;
	while(bloqueActual < bloquesTotales){
		Log().Get(tag, logINFO) << "Leyendo bloque: " << bloqueActual;
		lecturaActual = recibirInfo(sd, sizeof(int)); // Tamanio del bloque
		if(lecturaActual == NULL){
			return -1; // ERR: sin tamanio
		}
		int tamanioBloque = *(int*)lecturaActual;
		Log().Get(tag, logINFO) << "Tamanio del bloque: " << tamanioBloque;
		free(lecturaActual);
		lecturaActual = recibirInfo(sd, tamanioBloque);
		if(lecturaActual == NULL){
			return -1; // ERR: sin tamanio
		}
		serializable->deserialize(bloquesTotales, bloqueActual, lecturaActual);
		free(lecturaActual);
		bloqueActual ++;
	}
	return 1; // No error :D
}

