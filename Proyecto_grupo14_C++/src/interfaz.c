/*
 * interfaz.c
 *
 *  Created on: 31 mar 2025
 *      Author: a.azcorra
 */
#include <stdio.h>
#include "../includes/operaciones.h"
#include "../includes/GestorDB.h"
#include "../libs/sqlite3.h"
#include "../includes/interfaz.h"
#include "../includes/estructuras.h"
#include <stdlib.h>
#include <string.h>
#include "../includes/anadirDatosDB.h"
#include "../includes/finalInitDatabase.h"

void gestionarProductos(sqlite3 *db) {
	char opcion;

			do {
				printf("Bienvenido al gestor de productos\n");
			    printf("1. Mostrar productos\n");
			    printf("2. Añadir productos\n");
			    printf("3. Eliminar producto\n");
			    printf("4. Modificar producto\n");
			    printf("5. Ordenar productos\n");
			    //printf("6. Ver estanteria\n"); //esto es provisional hasta que la funcion funcione
			    printf("6. Buscar productos\n");
			    printf("Pulsar 'q' para salir\n");
			    printf("Seleccione una opción:\n");
			    fflush(stdout);
			    scanf(" %c", &opcion);

			    if(opcion == '1') {
			    	mostrarProductos(db);

			    }
			    else if(opcion == '2') {
			    	interfazAnadirProducto(db);
			    }
			    else if(opcion == '3') {
			    	interfazEliminarProducto(db);

			    }
			    else if(opcion == '4') {
			    	modificarProducto(db);
			    }
			    else if(opcion == '5') {
			    	mostrarProductosOrden(db);

			    }
			    /*
			    else if(opcion == '6') {
			    	//int numProds = obtenerIdUltimoProducto(db);
			    	//ordenarEstante(productos, numProds, &productosEstante, &num_secciones, &seccionesEstante);
			    	//mostrarProductosEstante(productosEstante, seccionesEstante, num_secciones);
			    	//liberarMemoria(productosEstante, num_secciones);
			    	//free(seccionesEstante);
			    }
			    */

			    else if (opcion == '6') {
			        buscarProductos(db);
			    }

			    } while (opcion != 'q');

}


void interfazAnadirProducto(sqlite3 *db){
	Producto producto;

	producto.idProd = obtenerIdUltimoProducto(db) + 1;

	printf("Introduce el nombre del producto: ");
	fflush(stdout);
	scanf("%s", producto.nombreProd);

	printf("Introduce el precio del producto: ");
	fflush(stdout);
	scanf("%f", &producto.precio);

	printf("Introduce la seccion del producto: ");
	fflush(stdout);
	scanf("%i", &producto.codSeccion);

	while (producto.codSeccion < 1 || producto.codSeccion > 6) {
		printf("Esa seccion no existe\n");
		printf("Las secciones disponibles son:\n");
		printf("1: limpieza\n");
		printf("2: congelados\n");
		printf("3: carniceria\n");
		printf("4: pescaderia\n");
		printf("5: cereales\n");
		printf("6: electrodomesticos\n");
		printf("Introduce la seccion del producto:\n");
		fflush(stdout);
		scanf("%i", &producto.codSeccion);
			   }
	printf("Proveedor del producto: ");
	fflush(stdout);
	scanf("%i", &producto.codProveedor);

	while (producto.codProveedor < 1 || producto.codProveedor > 6) {
		printf("Ese departamento no existe\n");
		printf("Los separtamentos disponibles son:\n");
		printf("1: Proveedor limpieza\n");
		printf("2: Proveedor congelados\n");
		printf("3: Proveedor carniceria\n");
		printf("4: Proveedor pescaderia\n");
		printf("5: Proveedor cereales\n");
		printf("6: Proveedor electrodomesticos\n");
		printf("Introduce el departamento del producto:\n");
		fflush(stdout);
		scanf("%i", &producto.codProveedor);
	};

	insertarProducto(db, producto);
	imprimirProducto(producto);

}

void interfazAnadirEmpleado(sqlite3 *db){
	Empleado empleado;

	printf("Introduce el NSS del empleado: ");
	fflush(stdout);
	scanf("%i", &empleado.NSS);

	while (empleado.NSS < 0000 || empleado.NSS > 9999) {
			printf("Ese NSS no es valido\n");
			printf("Introduce un numero entre 0000 y 9999\n");
			fflush(stdout);
			scanf("%i", &empleado.NSS);
		};

	printf("Introduce el nombre del empleado: ");
	fflush(stdout);
	scanf("%s", empleado.nombreEmpleado);

	printf("Introduce la contraseña del empleado: ");
	fflush(stdout);
	scanf("%s", empleado.contrasena);

	printf("Introduce id del departamento del empleado: ");
	fflush(stdout);
	scanf("%i", &empleado.idDepartamento);


	while (empleado.idDepartamento < 1 || empleado.idDepartamento > 6) {
		printf("Ese departamento no existe\n");
		printf("Los separtamentos disponibles son:\n");
		printf("1: Proveedor limpieza\n");
		printf("2: Proveedor congelados\n");
		printf("3: Proveedor carniceria\n");
		printf("4: Proveedor pescaderia\n");
		printf("5: Proveedor cereales\n");
		printf("6: Proveedor electrodomesticos\n");
		printf("Introduce el departamento del nuevo empleado:\n");
		fflush(stdout);
		scanf("%i", &empleado.idDepartamento);
	};

	printf("Introduce la id de la seccion del empleado: ");
	fflush(stdout);
	scanf("%i", &empleado.codSeccion);

	while (empleado.codSeccion < 1 || empleado.codSeccion > 6) {
		printf("Esa seccion no existe\n");
		printf("Las secciones disponibles son:\n");
		printf("1: limpieza\n");
		printf("2: congelados\n");
		printf("3: carniceria\n");
		printf("4: pescaderia\n");
		printf("5: cereales\n");
		printf("6: electrodomesticos\n");
		printf("Introduce la seccion del producto:\n");
		fflush(stdout);
		scanf("%i", &empleado.codSeccion);
			   }

	insertarEmpleado(db, empleado);

}

void interfazEliminarEmpleado(sqlite3 *db) {
	int NSS_empleado;

	printf("Introduce el NSS del empleado a eliminar: \n");
	fflush(stdout);
	scanf("%i", &NSS_empleado);
	eliminarEmpleado(db, NSS_empleado);
}



void interfazEliminarProducto(sqlite3 *db) {
	int idProd;

	int idMax = 0;
	idMax = obtenerIdUltimoProducto(db);

	printf("Introduce el id del producto a eliminar: \n");
	fflush(stdout);
	scanf("%i", &idProd);

	while(idProd < 1 || idProd > idMax) {
		printf("Este producto no existe\n");
		printf("Los productos disponibles van del 1 al %i\n", idMax);
		printf("Introduce el id del producto a eliminar: \n");
		fflush(stdout);
		scanf("%i", &idProd);
	}

		eliminarProducto(db, idProd);

}


void gestionarEmpleados(sqlite3 * db) {
	char opcion;

	do {
		printf("Bienvenido al gestor de empleados\n");
		printf("1. Mostrar empleados\n");
		printf("2. Añadir empleado\n");
		printf("3. Eliminar empleado\n");
		printf("4. Modificar empleado\n");
		printf("Pulsar 'q' para salir\n");
		printf("Seleccione una opción:\n");
		fflush(stdout);
		scanf(" %c", &opcion);

		if(opcion == '1') {
			mostrarEmpleados(db);

		}
		else if(opcion == '2') {
			interfazAnadirEmpleado(db);

		}
		else if(opcion == '3') {
			interfazEliminarEmpleado(db);

		}
		else if(opcion == '4') {
			modificarEmpleado(db);

		}
	}
	while (opcion != 'q');


}

void gestionarBBDD(sqlite3 * db, Config config) {  	// La constraseña es pass12
	Empleado e;
	obtenerAdministrativo(db, &e);

	printf("Hola %s con NSS %i, bienvenido a la administación de la BBDD de ADATE.\n", e.nombreEmpleado, e.NSS);
	printf("Ponga su contraseña antes de continuar:\n");
	fflush(stdout);

	char opcion;
	int contador = 1;
	int contrasenaCorrecta = 0;
	char contrasenya[50];
		do {
			scanf("%49s", contrasenya);
			if(strcmp(contrasenya, e.contrasena) == 0) {
//				menuGestionBBDD();
				printf("Bienvenido\n");
				fflush(stdout);
				contrasenaCorrecta = 1;
				interfazBBDD(db, &opcion, config);
			} else {

				contador += 1;
				if (contrasenaCorrecta == 0) {
					printf("Contraseña incorrecta.\n");
					fflush(stdout);
				}

				printf("Elige una opción:\n");
				printf("Pulsar 'q' para salir\n");
				fflush(stdout);
				if (contrasenaCorrecta == 0) {
					printf("Pulsar cualquier tecla para volver a intentarlo (SOLO UNA LETRA).\n");
					fflush(stdout);
				}

				scanf(" %c", &opcion);
				if (opcion != 'q') {
					printf("%iº intento, vuelva a introducir la contraseña:\n", contador);
					fflush(stdout);
				}

			 }



		}
		while (opcion != 'q');
}

void interfazBBDD(sqlite3* db, char *opOr, Config config) {
	char opcion;
	printf("Como jefe del departamento administrativo, puedes:\n");
		do {
			printf("Opciones:\n");
			printf("ATENCION: algunas opciones puede quitarte el poder administrativo si no eres el administrador principal!!!.\n");
			printf("1. Reiniciar la BBDD por completo\n");
			printf("2. Eliminar la BBDD\n");
			printf("3. Crear las tablas de la BBDD.\n");
			printf("4. Introducir los datos en la BBDD.\n");
			printf("5. Cambiar el administrador de este Eroski.\n");
			printf("Pulsar 'q' para salir\n");
			printf("Seleccione una opción:\n");
			fflush(stdout);
			scanf(" %c", &opcion);

			if(opcion == '1') {

				// EN LOS QUE NO HE PUESTO CONTROL DE ERROES ES POR QUE YA ESTÁ INTEGRADO EN SU FUNCIÓN.
				printf("Eliminando tablas antiguas...\n");
				dropTables(db);

				printf("Creando nuevas tablas...\n");

				crearTablasInit(db);


				introducirDatosDesdeCSV(db, config);


				printf("BASE DE DATOS REINICIADA\n");


			}
			else if(opcion == '2') {
				dropTables(db);
				printf("Base de datos eliminada\n");

			}
			else if(opcion == '3') {
				printf("Creando nuevas tablas...\n");

				crearTablasInit(db);

				printf("Tablas nuevas creadas\n");
				printf("ATENCIÓN: LAS TABLAS ESTÁS COMPLETAMENTE VACÍAS.\n");

			}
			else if(opcion == '4') {
				char opcion4;
				do {
					printf("ATENCION: si ya están los datos introducidos, puede a ver problemas porque posiblemente haya datos repetidos.\n");
					printf("¿Quíeres continuar?\n");
					printf("Sí o no (s/n)\n");
					fflush(stdout);
					scanf(" %s", &opcion4);
					if (opcion4 == 's') {
						introducirDatosDesdeCSV(db, config);
						opcion4 = 'n';
					}

				} while (opcion4 != 'n');

			 }
			else if(opcion == '5') {
				printf("Bienvenido al gestor del jefe del departamento administrativo.\n");
				editarJefeDepartamento(db, 7, config);
				printf("Gracias por usar nuestros servicios, hasta pronto.\n");
				opcion = 'q';
				*opOr ='q';

			}
			else if(opcion == 'q') {
				*opOr ='q';
			}
		}
		while (opcion != 'q');
}




