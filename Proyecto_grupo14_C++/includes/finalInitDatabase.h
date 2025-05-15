/*
 * finalInitDatabase.h
 *
 *  Created on: 4 abr 2025
 *      Author: j.fernandezde
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../libs/sqlite3.h"
#include "../includes/config.h"

#ifndef INCLUDES_FINALINITDATABASE_H_
#define INCLUDES_FINALINITDATABASE_H_

void crearTablasInit(sqlite3 *db);
void cargarProveedoresInit(sqlite3 *db, char* archivos_csv);
void cargarSeccionesInit(sqlite3 *db, char* archivos_csv);
void cargarDepartamentosInit(sqlite3 *db, char* archivos_csv);
void cargarEmpleadosInit(sqlite3 *db, char* archivos_csv);
void cargarProductosInit(sqlite3 *db, char* archivos_csv);
void introducirDatosDesdeCSV(sqlite3 *db, Config config);

#endif /* INCLUDES_FINALINITDATABASE_H_ */
