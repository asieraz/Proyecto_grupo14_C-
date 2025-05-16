#include "Producto.h"
#include <string.h>
#include <iostream>

Proveedor::Proveedor() {
	this->codProveedor = 0;
	this->nombreProveedor = NULL;
	this->codPostal = 0;
	this->contrasena = NULL;
}

Proveedor::Proveedor(int codProveedor, const char *nombreProveedor, int codPostal, const char *contrasena) {
	this->codProveedor = codProveedor;
	this->nombreProveedor = new char[strlen(nombreProveedor)+1];
	strcpy(this->nombreProveedor, nombreProveedor);
	this->codPostal = codPostal;
	this->contrasena = new char[strlen(contrasena)+1];
	strcpy(this->contrasena, contrasena);
}

Proveedor::Proveedor(const Proveedor &p) {
	this->codProveedor = p.codProveedor;
	this->nombreProveedor = new char[strlen(p.nombreProveedor)+1];
	strcpy(this->nombreProveedor, p.nombreProveedor);
	this->codPostal = p.codPostal;
	this->contrasena = new char[strlen(contrasena)+1];
	strcpy(this->contrasena, p.contrasena);
}

Proveedor::~Proveedor() {
	delete [] nombreProveedor;
	delete [] contrasena;
}

int Proveedor::getCodProveedor() {
	return this->codProveedor;
}

const char * Proveedor::getNombreProveedor() {
	return this->nombreProveedor;
}

int Proveedor::getCodPostal() {
	return this->codPostal;
}

const char * Proveedor::getContrasena() {
	return this->contrasena;
}

void Proveedor::setCodProveedor(int codProveedor) {
	this->codProveedor = codProveedor;
}

void Proveedor::setNombreProveedor(const char *nombreProveedor) {
	if(this->nombreProveedor != NULL) {
		delete [] this->nombreProveedor;
	}
	this->nombreProveedor = new char[strlen(nombreProveedor)+1];
	strcpy(this->nombreProveedor, nombreProveedor);
}

void Proveedor::setCodigoPostal(int codPostal) {
	this->codPostal = codPostal;
}

void Proveedor::setContrasena(const char *contrasena) {
	if(this->contrasena != NULL) {
		delete [] this->contrasena;
	}
	this->contrasena = new char[strlen(contrasena)+1];
	strcpy(this->contrasena, contrasena);
}


