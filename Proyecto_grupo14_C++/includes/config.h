/*
 * config.h
 *
 *  Created on: 9 abr 2025
 *      Author: a.azcorra
 */

#ifndef INCLUDES_CONFIG_H_
#define INCLUDES_CONFIG_H_

typedef struct {
    char database[256];
    char fichero_departamentos[256];
    char fichero_empleados[256];
    char fichero_productos[256];
    char fichero_proveedores[256];
    char fichero_secciones[256];
} Config;

void cargarConfig(const char* filename, Config* config);

#endif /* INCLUDES_CONFIG_H_ */
