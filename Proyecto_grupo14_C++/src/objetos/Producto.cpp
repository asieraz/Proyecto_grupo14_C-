#include <string.h>
#include <iostream>

#include "../../includes/objetos/headers_objetos.h"

Producto::Producto() {
	this->idProd = 0;
	this->nombreProd = NULL;
	this->precio = 0;
	this->codSeccion = 0;
	this->codProveedor = 0;
	this->tipo = NULL;
	this->numDisponibles = 0;
}

Producto::Producto(int idProd, const char *nombreProd, float precio, int codSeccion, int codProveedor, const char *tipo, int numDisponibles) {
	this->idProd = idProd;
	this->nombreProd = new char[strlen(nombreProd)+1];
	strcpy(this->nombreProd, nombreProd);
	this->precio = precio;
	this->codSeccion = codSeccion;
	this->codProveedor = codProveedor;
	this->tipo = new char[strlen(tipo)+1];
	strcpy(this->tipo, tipo);
	this->numDisponibles = numDisponibles;
}

Producto::Producto(const Producto &p) {
	this->idProd = p.idProd;
	this->nombreProd = new char[strlen(p.nombreProd)+1];
	strcpy(this->nombreProd, p.nombreProd);
	this->precio = p.precio;
	this->codSeccion = p.codSeccion;
	this->codProveedor = p.codProveedor;
	this->tipo = new char[strlen(p.tipo)+1];
	strcpy(this->tipo, p.tipo);
	this->numDisponibles = p.numDisponibles;
}

Producto::~Producto() {
	delete [] nombreProd;
	delete [] tipo;
}

int Producto::getIdProd() {
	return this->idProd;
}

const char *Producto::getNombreProd() {
	return this->nombreProd;
}

float Producto::getPrecio() {
	return this->precio;
}

int Producto::getCodSeccion() {
	return this->codSeccion;
}

int Producto::getCodProveedor() {
	return this->codProveedor;
}


int Producto::getNumDisponibles() {
	return this->numDisponibles;
}

void Producto::setIdProd(int idProd) {
	this->idProd = idProd;
}

void Producto::setNombreProd(const char *nombreProd) {
	if(this->nombreProd != NULL) {
		delete [] this->nombreProd;
		this->nombreProd = new char[strlen(nombreProd)+1];
		strcpy(this->nombreProd, nombreProd);
	}
}

void Producto::setPrecio(float precio) {
	this->precio = precio;
}

void Producto::setCodSeccion(int codSeccion) {
	this->codSeccion = codSeccion;
}

void Producto::setCodProveedor(int codProveedor) {
	this->codProveedor = codProveedor;
}

void Producto::setNumDisponibles(int numDisponibles) {
	this->numDisponibles = numDisponibles;
}

