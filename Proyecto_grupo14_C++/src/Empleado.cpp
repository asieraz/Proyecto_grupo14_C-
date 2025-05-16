#include "Producto.h"
#include <string.h>
#include <iostream>

Empleado::Empleado() {
	this->NSS = 0;
	this->nombreEmpleado = NULL;
	this->contrasena = NULL;
	this->codSeccion = 0;
	this->idDepartamento = 0;
}

Empleado::Empleado(int NSS, const char *nombreEmpleado, const char *contrasena, int codSeccion, int idDepartamento) {
	this->NSS = NSS;
	this->nombreEmpleado = new char[strlen(nombreEmpleado) + 1];
	strcpy(this->nombreEmpleado, nombreEmpleado);
	this->contrasena = new char[strlen(contrasena) + 1];
	strcpy(this->contrasena, contrasena);
	this->codSeccion = codSeccion;
	this->idDepartamento = idDepartamento;
}

Empleado::Empleado(const Empleado &e) {
	this->NSS = e.NSS;
	this->nombreEmpleado = new char[strlen(e.nombreEmpleado) + 1];
	strcpy(this->nombreEmpleado, e.nombreEmpleado);
	this->contrasena = new char[strlen(e.contrasena) + 1];
	strcpy(this->contrasena, e.contrasena);
	this->codSeccion = e.codSeccion;
	this->idDepartamento = e.idDepartamento;
}

Empleado::~Empleado() {
	delete [] nombreEmpleado;
	delete [] contrasena;
}

int Empleado::getNSS() {
	return this->NSS;
}

const char * Empleado::getNombreEmpleado() {
	return this->nombreEmpleado;
}

const char * Empleado::getContrasena() {
	return this->contrasena;
}

int Empleado::getCodSeccion() {
	return this->codSeccion;
}

int Empleado::getIdDepartamento() {
	return this->idDepartamento;
}

void Empleado::setNSS(int NSS) {
	this->NSS = NSS;
}

void Empleado::setNombreEmpleado(const char *nombreEmpleado) {
	if (this->nombreEmpleado != NULL) {
		delete [] this->nombreEmpleado;
	}
	this->nombreEmpleado = new char[strlen(nombreEmpleado) + 1];
	strcpy(this->nombreEmpleado, nombreEmpleado);
}

void Empleado::setContrasena(const char *contrasena) {
	if (this->contrasena != NULL) {
		delete [] this->contrasena;
	}
	this->contrasena = new char[strlen(contrasena) + 1];
	strcpy(this->contrasena, contrasena);
}

void Empleado::setCodSeccion(int codSeccion) {
	this->codSeccion = codSeccion;
}

void Empleado::setIdDepartamento(int idDepartamento) {
	this->idDepartamento = idDepartamento;
}


