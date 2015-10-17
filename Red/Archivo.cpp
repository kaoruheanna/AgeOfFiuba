/*
 * Archivo.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: nico
 */

#include "Archivo.h"

#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

using namespace std;

Archivo::~Archivo() {
	this->path = NULL;
	if(this->contenido != NULL){
		free(this->contenido);
		this->contenido = NULL;
	}
}

Archivo::Archivo(const char* archivo) {
	this->contenido = NULL;
	this->path = archivo;
}

void Archivo::cargarArchivo() {
	if(this->contenido == NULL){
		ifstream fin;
		fin.open(this->path, ios::binary | ios::in);
		if(fin.is_open()){
			fin.seekg(0, ios::end);
			size_t size = fin.tellg();
			this->contenido = (char*) malloc(size + 1);
			fin.seekg(0, ios::beg);
			fin.read(this->contenido, size);
			fin.close();
			this->contenido[size] = '\0';
		}
	}
}
// Serializable methods
int Archivo::getTotalBlockCount() {
	return 1;
}
int Archivo::getBlockSizeFromIndex(int currentIndex) {
	this->cargarArchivo();
	return strlen(this->contenido) + 1;
}
void Archivo::getBlockFromIndex(int currentIndex, void* buffer) {
	this->cargarArchivo();
	strcpy((char*) buffer, this->contenido);
	free(this->contenido);
	this->contenido = NULL;
}
void Archivo::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	ofstream fout;
	fout.open(this->path, ios::binary | ios::out);
	if(fout.is_open()){
		fout << (char*)blockData;
		fout.close();
	}
}
