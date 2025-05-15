/*
 * estructuras.h
 *
 *  Created on: 25 mar 2025
 *      Author: j.fernandezde
 */

#ifndef INCLUDES_ESTRUCTURAS_H_
#define INCLUDES_ESTRUCTURAS_H_

typedef struct {
	int idProd;
	char nombreProd[30];
	float precio;
	int codSeccion;
	int codProveedor;
}Producto;

typedef struct {
	int codProveedor;
	char nombreProveedor[30];
	int codPostal;
	char contrasena[30];
}Proveedor;

typedef struct {
	int codSeccion;
	char nombreSeccion[30];
}Seccion;

typedef struct {
	int idDepartamento;
	char nombreDepartamento[30];
	int NSSJefe;
}Departamento;

typedef struct {
	int NSS;
	char nombreEmpleado[30];
	char contrasena[30];
	int codSeccion;
	int idDepartamento;
}Empleado;

#endif /* INCLUDES_ESTRUCTURAS_H_ */
