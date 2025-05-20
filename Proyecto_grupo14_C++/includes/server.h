/*
 * server.h
 *
 *  Created on: 20 may 2025
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

#ifdef __cplusplus
extern "C" {
#endif

void run_server(sqlite3 *db);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDES_SERVER_H_ */
