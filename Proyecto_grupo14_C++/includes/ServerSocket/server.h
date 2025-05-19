/*
 * server.h
 *
 *  Created on: 17 may 2025
 *      Author: a.azcorra
 */

#ifndef INCLUDES_SERVER_H_
#define INCLUDES_SERVER_H_


#include "../includes/finalInitDatabase.h"
#include "../includes/anadirDatosDB.h"
#include "../includes/GestorDB.h"
#include "../libs/sqlite3.h"
#include "../includes/interfaz.h"
#include "../includes/estructuras.h"
#include "../includes/config.h"

void run_server(sqlite3 *db);

#endif /* INCLUDES_SERVER_H_ */


