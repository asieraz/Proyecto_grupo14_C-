/*
 * server.h
 *
 *  Created on: 17 may 2025
 *      Author: a.azcorra
 */

#ifndef INCLUDES_SERVER_H_
#define INCLUDES_SERVER_H_


#include "finalInitDatabase.h"
#include "anadirDatosDB.h"
#include "gestorDB.h"
#include "../libs/sqlite3.h"
#include "interfaz.h"
#include "estructuras.h"
#include "config.h"

void run_server(sqlite3 *db);

#endif /* INCLUDES_SERVER_H_ */


