#include "Producto.h"
#include <string.h>
#include <iostream>

Seccion::Seccion() {
	this->codSeccion = 0;
	this->nombreSeccion = NULL;
}

Seccion::Seccion(int codSeccion, const char *nombreSeccion) {
	this->codSeccion = codSeccion;
	this->nombreSeccion = new char[strlen(nombreSeccion)+1];
	strcpy(this->nombreSeccion, nombreSeccion);
}

Seccion::Seccion(const Seccion &s) {
	this->codSeccion = s.codSeccion;
	this->nombreSeccion = new char[strlen(s.nombreSeccion)+1];
	strcpy(this->nombreSeccion, s.nombreSeccion);
}

Seccion::~Seccion() {
	delete [] nombreSeccion;
}

int Seccion::getCodSeccion() {
	return this->codSeccion;
}

const char * Seccion::getNombreSeccion() {
	return this->nombreSeccion;
}

void Seccion::setCodSeccion(int codSeccion) {
	this->codSeccion = codSeccion;
}

void Seccion::setNombreSeccion(const char *nombreSeccion) {
	if(this->nombreSeccion != NULL) {
		delete [] this->nombreSeccion;
	}
	this->nombreSeccion = new char[strlen(nombreSeccion)+1];
	strcpy(this->nombreSeccion, nombreSeccion);
}


