/*
 * config.c
 *
 *  Created on: 9 abr 2025
 *      Author: a.azcorra
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/config.h"

void cargarConfig(const char* filename, Config* config) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("No se puede abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* key = strtok(line, ": ");
        char* value = strtok(NULL, "\n");

        if (strcmp(key, "database") == 0) {
            strcpy(config->database, value);
        } else if (strcmp(key, "fichero_departamentos") == 0) {
            strcpy(config->fichero_departamentos, value);
        } else if (strcmp(key, "fichero_empleados") == 0) {
            strcpy(config->fichero_empleados, value);
        }else if (strcmp(key, "fichero_productos") == 0) {
            strcpy(config->fichero_productos, value);
        }else if (strcmp(key, "fichero_proveedores") == 0) {
            strcpy(config->fichero_proveedores, value);
        }else if (strcmp(key, "fichero_secciones") == 0) {
            strcpy(config->fichero_secciones, value);
        }
    }

    fclose(file);
}
