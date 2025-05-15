/*
 * GestorDB.c
 *
 *  Created on: 25 mar 2025
 *      Author: j.fernandezde
 */
#include "../../includes/GestorDB.h"
#include "../../includes/interfaz.h"

#include <stdio.h>
#include "../../libs/sqlite3.h"
#include <stdlib.h>
#include <string.h>

void dropTables(sqlite3 * db) {
    char *errMsg = 0;
    char *sql = "DROP TABLE IF EXISTS producto; "
    			"DROP TABLE IF EXISTS departamento; "
                "DROP TABLE IF EXISTS proveedor; "
    			"DROP TABLE IF EXISTS empleado; "
                "DROP TABLE IF EXISTS seccion; ";

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        printf("Error al borrar tablas: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Tablas eliminadas correctamente\n");
    }
}


void mostrarProductos(sqlite3 *db) {

	sqlite3_stmt *stmt;

	char sql[] = "select id_Producto, nombre, precio, id_Proveedor, cod_Seccion from producto";

	int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparando la consulta SELECT\n");
	    printf("%s\n", sqlite3_errmsg(db));
	    return;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int idProd = sqlite3_column_int(stmt, 0);
		const unsigned char *nombreProd = sqlite3_column_text(stmt, 1);
		double precio = sqlite3_column_double(stmt, 2);
	    int codProveedor = sqlite3_column_int(stmt, 3);
	    int codSeccion = sqlite3_column_int(stmt, 4);

	    if(!idProd == 0) {
	    	printf("Producto: %d, Nombre: %s, Precio: %.2f, Proveedor: %d, Sección: %d\n", idProd, nombreProd, precio, codProveedor, codSeccion);
	    }
	}


	sqlite3_finalize(stmt);

}


void eliminarEmpleado(sqlite3 * db, int NSS_empleado) {
	char * mensajeError = 0;
	char sql[256];

	sprintf(sql, "DELETE FROM empleado WHERE NSS_Empleado = %i", NSS_empleado);

	if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
		printf("Error al eliminar el empleado: %s\n", mensajeError);
		sqlite3_free(mensajeError);
	} else {
		printf("Empleado eliminado: %i\n", NSS_empleado);
	}
}


void eliminarProducto(sqlite3 * db, int idProd) {
	char * mensajeError = 0;
	char sql[256];
	int contador = obtenerIdUltimoProducto(db);
	contador -= 1;

	sprintf(sql, "DELETE FROM producto WHERE id_Producto = %i", idProd);

	if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
		printf("Error al eliminar el producto: %s\n", mensajeError);
		sqlite3_free(mensajeError);
	} else {
		printf("Producto eliminado: %i\n", idProd);
	}
}

void modificarProducto(sqlite3 *db) {
	char * mensajeError = 0;
	char sql[256];
	int contador = obtenerIdUltimoProducto(db);
	int id;
	char nuevoNombre[50];
	float nuevoPrecio;

	printf("Ingrese el ID del producto a modificar: ");
	fflush(stdout);
	scanf("%d", &id);

	while(id < 1 || id > contador) {
		printf("ID incorrecto. Eliga id entre 1 y %i: ", contador);
		fflush(stdout);
		scanf("%d", &id);
	}

	printf("Ingrese el nuevo nombre: ");
	fflush(stdout);
	scanf("%s", &nuevoNombre);

	printf("Ingrese el nuevo precio: ");
	fflush(stdout);
	scanf("%f", &nuevoPrecio);

	sprintf(sql, "UPDATE producto SET nombre = '%s', precio = %.2f WHERE id_Producto = %d;", nuevoNombre, nuevoPrecio, id);

	if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
		printf("Error al eliminar el producto: %s\n", mensajeError);
		sqlite3_free(mensajeError);
	} else {
		printf("Producto modificado: %i , nombre: %s\n", id, nuevoNombre);
	}

}

void modificarEmpleado(sqlite3 *db) {
	char * mensajeError = 0;
	char sql[256];
	int NSS;
	char nuevoNombre[50];
	char nuevaContrasena[50];
	int idDepartamento;
	int idSeccion;

	printf("Ingrese el NSS del empleado a modificar:\n ");
	fflush(stdout);
	scanf("%i", &NSS);

	printf("Ingrese el nuevo nombre:\n ");
	fflush(stdout);
	scanf("%s", nuevoNombre);

	printf("Ingrese la nueva contraseña:\n ");
	fflush(stdout);
	scanf("%s", nuevaContrasena);

	printf("Introduce la nueva id de departamento:\n ");
	fflush(stdout);
	scanf("%i", &idDepartamento);

	while (idDepartamento < 1 || idDepartamento > 6) {
		printf("Ese departamento no existe\n");
		printf("Los separtamentos disponibles son:\n");
		printf("1: Proveedor limpieza\n");
		printf("2: Proveedor congelados\n");
		printf("3: Proveedor carniceria\n");
		printf("4: Proveedor pescaderia\n");
		printf("5: Proveedor cereales\n");
		printf("6: Proveedor electrodomesticos\n");
		printf("Introduce la nueva id de departamento:\n");
		fflush(stdout);
		scanf("%i", &idDepartamento);
	};

	printf("Introduce la id nueva id de seccion:\n ");
		fflush(stdout);
		scanf("%i", &idSeccion);

		while (idSeccion < 1 || idSeccion > 6) {
			printf("Esa seccion no existe\n");
			printf("Las secciones disponibles son:\n");
			printf("1: limpieza\n");
			printf("2: congelados\n");
			printf("3: carniceria\n");
			printf("4: pescaderia\n");
			printf("5: cereales\n");
			printf("6: electrodomesticos\n");
			printf("Introduce la id nueva id de seccion:\n");
			fflush(stdout);
			scanf("%i", &idSeccion);
				   }

	sprintf(sql, "UPDATE empleado SET nombre = '%s', contrasena = '%s', id_Departamento = %i, id_Seccion = %i  WHERE NSS_Empleado = %i;", nuevoNombre, nuevaContrasena, idDepartamento, idSeccion, NSS);

	if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
		printf("Error al modificar el empleado: %s\n", mensajeError);
		sqlite3_free(mensajeError);
	} else {
		printf("Empleado modificado: NSS: %i , nombre: %s, contraseña: %s, departamento: %i, seccion: %i\n", NSS, nuevoNombre, nuevaContrasena, idDepartamento, idSeccion);
	}

}

void buscarProductos(sqlite3 *db) {
	int opcion;
	char nombreBuscador[50];

	printf("Elige el modo de busqueda: \n");
	printf("1: Buscar por nombre \n");
	printf("2: Buscar por seccion\n");
	fflush(stdout);
	scanf("%i", &opcion);
	getchar();
	fflush(stdout);

	if(opcion == 1) {
		char sql[256];

		printf("Elige el nombre del producto: \n");
		fflush(stdout);
		scanf("%s", &nombreBuscador);
		getchar();
		fflush(stdout);

		 snprintf(sql, sizeof(sql), "SELECT id_Producto, nombre, precio, id_Proveedor, cod_Seccion FROM producto WHERE nombre LIKE '%%%s%%';", nombreBuscador);

		 sqlite3_stmt *stmt;
		 if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
			 printf("\nResultados de la búsqueda por nombre:\n");
			 while (sqlite3_step(stmt) == SQLITE_ROW) {
				 int id = sqlite3_column_int(stmt, 0);
				 const unsigned char *nombre = sqlite3_column_text(stmt, 1);
				 float precio = (float)sqlite3_column_double(stmt, 2);
				 int proveedor = sqlite3_column_int(stmt, 3);
				 int seccion = sqlite3_column_int(stmt, 4);

				 if(id != 0) {
					 printf("ID: %d | Nombre: %s | Precio: %.2f | Proveedor: %d | Sección: %d\n", id, nombre, precio, proveedor, seccion);
				 }
			 }
			 sqlite3_finalize(stmt);
		     }
		 else {
			 printf("Error en la consulta: %s\n", sqlite3_errmsg(db));
		 	 }

	}
	else if(opcion == 2) {
		char sql[256];

		printf("Elige la seccion del producto: \n");
		fflush(stdout);
		scanf("%s", &nombreBuscador);
		getchar();
		fflush(stdout);

		snprintf(sql, sizeof(sql), "SELECT id_Producto, nombre, precio, id_Proveedor FROM producto WHERE cod_Seccion = %s;", nombreBuscador);

		sqlite3_stmt *stmt;
		if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
			printf("\nResultados de la búsqueda por categoría:\n");
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				int id = sqlite3_column_int(stmt, 0);
		        const unsigned char *nombre = sqlite3_column_text(stmt, 1);
		        float precio = (float)sqlite3_column_double(stmt, 2);
		        int proveedor = sqlite3_column_int(stmt, 3);

		        if(id != 0) {
		        	printf("ID: %d | Nombre: %s | Precio: %.2f | Proveedor: %d\n", id, nombre, precio, proveedor);
		        }
				}
			sqlite3_finalize(stmt);
			}
		else {
			printf("Error en la consulta: %s\n", sqlite3_errmsg(db));
		}
	}
	else {
		printf("Opcion no valida \n");
	}
}

void mostrarProductosOrden(sqlite3 *db) {

	sqlite3_stmt *stmt;

	char sql[] = "select id_Producto, nombre, precio, id_Proveedor, cod_Seccion from producto order by precio DESC";

	int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparando la consulta SELECT\n");
		printf("%s\n", sqlite3_errmsg(db));
		return;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int idProd = sqlite3_column_int(stmt, 0);
		const unsigned char *nombreProd = sqlite3_column_text(stmt, 1);
		double precio = sqlite3_column_double(stmt, 2);
		int codProveedor = sqlite3_column_int(stmt, 3);
		int codSeccion = sqlite3_column_int(stmt, 4);

		if(!idProd == 0) {
			printf("Producto: %d, Nombre: %s, Precio: %.2f, Proveedor: %d, Sección: %d\n", idProd, nombreProd, precio, codProveedor, codSeccion);
		}
	}


	sqlite3_finalize(stmt);


}


int obtenerIdUltimoProducto(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT MAX(id_Producto) FROM producto;";
    int maximo = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maximo = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        printf("Error al obtener la id del ultimo producto: %s\n", sqlite3_errmsg(db));
    }

    return maximo;
}

void mostrarEmpleados(sqlite3 *db) {
	sqlite3_stmt *stmt;

	char sql[] = "select NSS_Empleado, nombre, id_Departamento, id_Seccion from empleado";

	int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparando la consulta SELECT\n");
		printf("%s\n", sqlite3_errmsg(db));
		return;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int NSS = sqlite3_column_int(stmt, 0);
		const unsigned char *nombreEmp = sqlite3_column_text(stmt, 1);
		int codDepartamento = sqlite3_column_int(stmt, 2);
		int codSeccion = sqlite3_column_int(stmt, 3);

		printf("Empleado: %d, Nombre: %s,, Departamento: %d, Sección: %d\n", NSS, nombreEmp, codDepartamento, codSeccion);
	}

	sqlite3_finalize(stmt);

}

void obtenerAdministrativo(sqlite3 *db, Empleado *e) {
    sqlite3_stmt *stmt;

    char sql[] = "SELECT E.NSS_EMPLEADO, E.NOMBRE, E.CONTRASENA, E.ID_DEPARTAMENTO, E.ID_SECCION "
                 "FROM EMPLEADO E, DEPARTAMENTO D "
                 "WHERE D.NSS_JEFE = E.NSS_EMPLEADO AND D.NOMBRE = 'Administrativo'";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando la consulta SELECT\n");
        printf("%s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        e->NSS = sqlite3_column_int(stmt, 0);
        strcpy(e->nombreEmpleado, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(e->contrasena, (const char *)sqlite3_column_text(stmt, 2));
        e->idDepartamento = sqlite3_column_int(stmt, 3);
        e->codSeccion = sqlite3_column_int(stmt, 4);


    } else {
        printf("No se encontró ningún empleado administrativo.\n");
    }

    sqlite3_finalize(stmt);
}

void editarJefeDepartamento(sqlite3 *db, int numDept, Config config) {
    char *mensajeError = 0;
    char sql[150];
    int nuevoNSS;

    printf("Introduce el NSS del nuevo jefe del departamento %i\n", numDept);
    fflush(stdout);
    scanf("%i", &nuevoNSS);
    sprintf(sql, "UPDATE departamento SET NSS_Jefe = %i WHERE id_Departamento = %i", nuevoNSS, numDept);

    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        printf("Error al actualizar el jefe del departamento %i: %s\n", numDept, mensajeError);
        sqlite3_free(mensajeError);
    } else {
        printf("El nuevo jefe es %i\n", nuevoNSS);
        fflush(stdout);
        gestionarBBDD(db, config);

    }
}

