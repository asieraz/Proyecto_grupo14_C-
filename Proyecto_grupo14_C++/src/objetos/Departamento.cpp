#include <string.h>
#include <iostream>

#include "../../includes/objetos/headers_objetos.h"

Departamento::Departamento() {
	this->idDepartamento = 0;
	this->nombreDepartamento = NULL;
	this->NSSJefe = 0;
}

Departamento::Departamento(int idDepartamento, const char *nombreDepartamento, int NSSJefe) {
	this->idDepartamento = idDepartamento;
	this->nombreDepartamento = new char[strlen(nombreDepartamento)+1];
	strcpy(this->nombreDepartamento, nombreDepartamento);
	this->NSSJefe = NSSJefe;
}

Departamento::Departamento(const Departamento &d) {
	this->idDepartamento = d.idDepartamento;
	this->nombreDepartamento = new char[strlen(d.nombreDepartamento)+1];
	strcpy(this->nombreDepartamento, d.nombreDepartamento);
	this->NSSJefe = d.NSSJefe;
}

Departamento::~Departamento() {
	delete [] nombreDepartamento;
}

int Departamento::getIdDepartamento() {
	return this->idDepartamento;
}

const char * Departamento::getNombreDepartamento() {
	return this->nombreDepartamento;
}

int Departamento::getNSSJefe() {
	return this->NSSJefe;
}

void Departamento::setIdDepartamento(int idDepartamento) {
	this->idDepartamento = idDepartamento;
}

void Departamento::setNombreDepartamento(const char *nombreDepartamento) {
	if(this->nombreDepartamento != NULL) {
		delete [] this->nombreDepartamento;
	}
	this->nombreDepartamento = new char[strlen(nombreDepartamento)+1];
	strcpy(this->nombreDepartamento, nombreDepartamento);
}

void Departamento::setNSSJefe(int NSSJefe) {
	this->NSSJefe = NSSJefe;
}


