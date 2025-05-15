/*
 * interfaz.h
 *
 *  Created on: 31 mar 2025
 *      Author: a.azcorra
 */

#ifndef INCLUDES_INTERFAZ_H_
#define INCLUDES_INTERFAZ_H_

#include "../libs/sqlite3.h"
#include "../includes/estructuras.h"
#include "config.h"

void gestionarProductos(sqlite3 *db);

void gestionarEmpleados(sqlite3 *db);

void gestionarBBDD(sqlite3 *db, Config config);

void interfazEliminarEmpleado(sqlite3 *db);

void interfazAnadirProducto(sqlite3 *db);

void interfazAnadirEmpleado(sqlite3 *db);

void interfazEliminarProducto(sqlite3 *db);

void interfazBBDD(sqlite3 *db, char* opOr, Config config);

#endif /* INCLUDES_INTERFAZ_H_ */
