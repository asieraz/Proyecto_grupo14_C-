/*
 * GestorBD.h
 *
 *  Created on: 25 mar 2025
 *      Author: j.fernandezde
 */

#ifndef SRC_DB_MANAGER_GESTORDB_H_
#define SRC_DB_MANAGER_GESTORDB_H_

#include "../libs/sqlite3.h"
#include "../includes/estructuras.h"
#include "config.h"

void dropTables(sqlite3 *db);

void mostrarProductos(sqlite3 *db);

void eliminarEmpleado(sqlite3 * db, int NSS_empleado);

void anadirProducto (sqlite3 * db, Producto producto);

void eliminarProducto(sqlite3 * db, int idProd);

void modificarProducto(sqlite3 *db);

void modificarEmpleado(sqlite3 *db);

void buscarProductos(sqlite3 *db);

void mostrarProductosOrden(sqlite3 *db);

int obtenerIdUltimoProducto(sqlite3 *db);

void mostrarEmpleados(sqlite3 *db);

void obtenerAdministrativo(sqlite3 *db, Empleado *e);

void editarJefeDepartamento(sqlite3 *db, int i, Config config);
#endif /* SRC_DB_MANAGER_GESTORDB_H_ */
