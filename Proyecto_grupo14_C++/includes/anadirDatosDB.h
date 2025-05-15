/*
 * anadirDatosDB.h
 *
 *  Created on: 5 abr 2025
 *      Author: a.azcorra
 */

#ifndef INCLUDES_ANADIRDATOSDB_H_
#define INCLUDES_ANADIRDATOSDB_H_

#include "../libs/sqlite3.h"
#include "../includes/estructuras.h"

void crearTablas(sqlite3 *db);

void insertarDepartamento(sqlite3 *db, Departamento departamento);
void insertarEmpleado(sqlite3 *db, Empleado empleado);
void insertarProducto(sqlite3 *db, Producto producto);
void insertarProveedor(sqlite3 *db, Proveedor proveedor);
void insertarSeccion(sqlite3 *db, Seccion seccion);

//void cargarFicheros (sqlite3 *db);

#endif /* INCLUDES_ANADIRDATOSDB_H_ */
