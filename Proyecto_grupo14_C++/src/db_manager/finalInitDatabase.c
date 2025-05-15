/*
 * finalInitDatabase.c
 *
 *  Created on: 4 abr 2025
 *      Author: j.fernandezde
 */


#include <stdio.h>
#include "../../includes/finalInitDatabase.h"
#include "../../libs/sqlite3.h"
#include "../../includes/config.h"
#include <stdlib.h>
#include <string.h>

void crearTablasInit(sqlite3 *db) {

	char * mensajeError = 0;

	const char * sql =

			"PRAGMA foreign_keys = ON;"

			"CREATE TABLE IF NOT EXISTS proveedor ("
			"id_Proveedor INTEGER PRIMARY KEY, "
			"nombre TEXT, "
			"codigo_Postal INTEGER, "
			"contrasena TEXT"
			");"

			"CREATE TABLE IF NOT EXISTS seccion ("
			"cod_seccion INTEGER PRIMARY KEY, "
			"nombre TEXT"
			");"

			"CREATE TABLE IF NOT EXISTS empleado ("
			"NSS_Empleado INTEGER PRIMARY KEY, "
			"nombre TEXT, "
			"contrasena TEXT, "
			"id_Departamento INTEGER, "
			"id_Seccion INTEGER, "
			"FOREIGN KEY(id_Departamento) REFERENCES departamento(id_Departamento) ON DELETE CASCADE, "
			"FOREIGN KEY(id_Seccion) REFERENCES seccion(cod_seccion) ON DELETE CASCADE"
			");"

			"CREATE TABLE IF NOT EXISTS departamento ("
			"id_Departamento INTEGER PRIMARY KEY, "
			"nombre TEXT, "
			"NSS_Jefe INTEGER "
			");"

			"CREATE TABLE IF NOT EXISTS producto ("
			"id_Producto INTEGER PRIMARY KEY, "
			"nombre TEXT, "
			"precio REAL, "
			"id_Proveedor INTEGER, "
			"cod_Seccion INTEGER, "
			"FOREIGN KEY(id_Proveedor) REFERENCES proveedor(id_Proveedor) ON DELETE CASCADE, "
			"FOREIGN KEY(cod_Seccion) REFERENCES seccion(cod_seccion) ON DELETE CASCADE"
			");";


	int ejecutar = sqlite3_exec(db, sql, 0, 0, &mensajeError);

	if(ejecutar != SQLITE_OK) {
		printf("Error al crear las tablas: %s\n", mensajeError);
		sqlite3_free(mensajeError);
	}
	else {
		printf("Tablas creadas correctamente\n");
	}

}

//void cargarProveedoresInit(sqlite3 *db) {
//	char archivos_csv[] = "database/datosIniciales/proveedores.csv";
//    FILE *archivo = fopen(archivos_csv, "r");
//    if (!archivo) {
//        perror("No se pudo abrir proveedores.csv");
//        return;
//    }
//
//    char *error_msg = NULL;
//    char linea[256];
//
//    while (fgets(linea, sizeof(linea), archivo)) {
//        int id;
//        char nombre[31], contrasena[31];
//        int codigo_postal;
//
//        // Quitar salto de línea
////        linea[strcspn(linea, "\n")] = 0;
//
//        // Parsear los valores
//        if (sscanf(linea, "%d,%30[^,],%d,%30[^\n]", &id, nombre, &codigo_postal, contrasena) != 4) {
//            fprintf(stderr, "Error al parsear línea: %s\n", linea);
//            continue;
//        }
//
//        // Crear consulta
//        char consulta[512];
//        snprintf(consulta, sizeof(consulta),
//                 "INSERT INTO proveedor (id_Proveedor, nombre, codigo_Postal, contrasena) "
//                 "VALUES (%d, '%s', %d, '%s');",
//                 id, nombre, codigo_postal, contrasena);
//
//        // Ejecutar consulta
//        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
//            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
//            sqlite3_free(error_msg);
//        }
//    }
//    fclose(archivo);
//
//}

void cargarProveedoresInit(sqlite3 *db, char* archivos_csv) {
    FILE *archivo = fopen(archivos_csv, "r");
    if (!archivo) {
        perror("No se pudo abrir proveedores.csv");
        return;
    }

    char *error_msg = NULL;
    char *linea;
    linea = malloc(256 * sizeof(char));  // Prueba con memoria dinámica.

    while (fgets(linea, 256 + sizeof(char), archivo)) {
        int id;
        char nombre[31], contrasena[31];
        int codigo_postal;

        // Quitar salto de línea
//        linea[strcspn(linea, "\n")] = 0;

        // Parsear los valores
        if (sscanf(linea, "%d,%30[^,],%d,%30[^\n]", &id, nombre, &codigo_postal, contrasena) != 4) {
            fprintf(stderr, "Error al parsear línea: %s\n", linea);
            continue;
        }

        // Crear consulta
        char consulta[512];
        snprintf(consulta, sizeof(consulta),
                 "INSERT OR IGNORE INTO proveedor (id_Proveedor, nombre, codigo_Postal, contrasena) "
                 "VALUES (%d, '%s', %d, '%s');",
                 id, nombre, codigo_postal, contrasena);

        // Ejecutar consulta
        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
            sqlite3_free(error_msg);
        }
    }
    free(linea);
    fclose(archivo);

}

void cargarSeccionesInit(sqlite3 *db, char* archivos_csv) {
    FILE *archivo = fopen(archivos_csv, "r");
    if (!archivo) {
        perror("No se pudo abrir secciones.csv");
        return;
    }

    char *error_msg = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), archivo)) {
        int cod_seccion;
        char nombre[31];


        // Parsear los valores
        if (sscanf(linea, "%d,%30[^\n]", &cod_seccion, nombre) != 2) {
            fprintf(stderr, "Error al parsear línea: %s\n", linea);
            continue;
        }

        // Crear consulta
        char consulta[512];
        snprintf(consulta, sizeof(consulta),
                 "INSERT OR IGNORE INTO seccion (cod_seccion, nombre) "
                 "VALUES (%d, '%s');",
                 cod_seccion, nombre);

        // Ejecutar consulta
        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
            sqlite3_free(error_msg);
        }
    }
    fclose(archivo);

}

void cargarDepartamentosInit(sqlite3 *db, char* archivos_csv) {
    FILE *archivo = fopen(archivos_csv, "r");
    if (!archivo) {
        perror("No se pudo abrir departamentos.csv");
        return;
    }

    char *error_msg = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), archivo)) {
        int id;
        char nombre[50];
        int NSS_Jefe;

        // Quitar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        // Parsear los valores
        if (sscanf(linea, "%d,%30[^,],%d", &id, nombre, &NSS_Jefe) != 3) {		// 3 es la cantidad de caracteres esperados.
            fprintf(stderr, "Error al parsear línea: %s\n", linea);
            continue;
        }
        // Crear consulta
        char consulta[512];
        snprintf(consulta, sizeof(consulta),
                 "INSERT OR IGNORE INTO departamento (id_Departamento, nombre, NSS_Jefe) "
                 "VALUES (%d, '%s', %d);",
                 id, nombre, NSS_Jefe);

        // Ejecutar consulta
        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
            sqlite3_free(error_msg);
        }
    }

    fclose(archivo);
}

void cargarEmpleadosInit(sqlite3 *db, char* archivos_csv) {
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, NULL);
    FILE *archivo = fopen(archivos_csv, "r");
    if (!archivo) {
        perror("No se pudo abrir empleados.csv");
        return;
    }

    char *error_msg = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), archivo)) {
        int NSS_Empleado;
        char nombre[30];
        char contrasenya[30];
        int id_Departamento;
        int id_Seccion;

        // Quitar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        // Parsear los valores
        if (sscanf(linea, "%d,%29[^,],%29[^,],%d,%d", &NSS_Empleado, nombre, contrasenya, &id_Departamento, &id_Seccion) != 5) {		// 5 es la cantidad de caracteres esperados.
            fprintf(stderr, "Error al parsear línea: %s\n", linea);
            continue;
        }

//        if (sscanf(linea, "%[^,],%[^,],%[^,],%d,%d\n", &NSS_Empleado, nombre, contrasenya, &id_Departamento, &id_Seccion) != 5) {		// 5 es la cantidad de caracteres esperados.
//                    fprintf(stderr, "Error al parsear línea: %s\n", linea);
//                    continue;
//                }

        // Crear consulta
        char consulta[512];
        snprintf(consulta, sizeof(consulta),
                 "INSERT OR IGNORE INTO empleado (NSS_Empleado, nombre, contrasena, id_Departamento, id_Seccion) "
                 "VALUES (%d, '%s', '%s', %d, %d);",
				 NSS_Empleado, nombre, contrasenya, id_Departamento, id_Seccion);

        // Ejecutar consulta
        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
            sqlite3_free(error_msg);
        }
    }

    fclose(archivo);
}

void cargarProductosInit(sqlite3 *db, char* archivos_csv) {
    FILE *archivo = fopen(archivos_csv, "r");
    if (!archivo) {
        perror("No se pudo abrir productos.csv");
        return;
    }

    char *error_msg = NULL;
    char linea[256];

    while (fgets(linea, sizeof(linea), archivo)) {
        int id_Producto;
        char nombre[30];
        float precio;
        int id_Proveedor;
        int cod_Seccion;

        // Quitar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        // Parsear los valores (ajustado para la estructura de productos)
        if (sscanf(linea, "%d,%49[^,],%f,%d,%d",
                   &id_Producto, nombre, &precio, &id_Proveedor, &cod_Seccion) != 5) {
            fprintf(stderr, "Error al parsear línea: %s\n", linea);
            continue;
        }

        // Crear consulta SQL para productos
        char consulta[512];
        snprintf(consulta, sizeof(consulta),
                 "INSERT OR IGNORE INTO producto (id_Producto, nombre, precio, id_Proveedor, cod_Seccion) "
                 "VALUES (%d, '%s', %.2f, %d, %d);",
                 id_Producto, nombre, precio, id_Proveedor, cod_Seccion);

        // Ejecutar consulta
        if (sqlite3_exec(db, consulta, 0, 0, &error_msg) != SQLITE_OK) {
            fprintf(stderr, "Error en INSERT: %s\n", error_msg);
            sqlite3_free(error_msg);
        }
    }

    fclose(archivo);
}

void introducirDatosDesdeCSV(sqlite3 *db, Config config) {
	printf("Introduciendo datos base desde los CSVs...\n");

		cargarProveedoresInit(db, config.fichero_proveedores);
		printf("Proveedores añadidos\n");


		cargarSeccionesInit(db, config.fichero_secciones);
		printf("Secciones añadidas\n");


		cargarDepartamentosInit(db, config.fichero_departamentos);
		printf("Departamentos añadidos\n");


		cargarEmpleadosInit(db, config.fichero_empleados);
		printf("Empleados añadidos\n");


		cargarProductosInit(db, config.fichero_productos);
		printf("Productos añadidos\n");

}
