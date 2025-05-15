#include <stdio.h>
#include "../includes/operaciones.h"
#include "../libs/sqlite3.h"
#include "../includes/anadirDatosDB.h"
#include <stdlib.h>
#include <string.h>



int compararPorPrecio(const void *a, const void *b) {
	Producto *p1 = (Producto *)a;
	Producto *p2 = (Producto *)b;


	if (p1->precio < p2->precio) return 1;
	if (p1->precio > p2->precio) return -1;
	return 0;
}

void ordenarEstante(Producto *productos, int num_productos, Producto ***productosEstante, int * num_secciones, int ** seccionesEstante) {

	*num_secciones = contarSecciones(productos, num_productos, seccionesEstante);

	*productosEstante = malloc((*num_secciones) * sizeof(Producto *));
	int *contadorSeccion = calloc((*num_secciones), sizeof(int));

	for (int i = 0; i < num_productos; i++) {
		for (int j = 0; j < *num_secciones; j++) {
			if (productos[i].codSeccion == (*seccionesEstante)[j]) {
				contadorSeccion[j]++;
	            break;
	        }
		}
	}

	for (int i = 0; i < *num_secciones; i++) {
		(*productosEstante)[i] = malloc(contadorSeccion[i] * sizeof(Producto));
		contadorSeccion[i] = 0;
	}

	for (int i = 0; i < num_productos; i++) {
		for (int j = 0; j < *num_secciones; j++) {
			if (productos[i].codSeccion == (*seccionesEstante)[j]) {
				(*productosEstante)[j][contadorSeccion[j]++] = productos[i];
				break;
			}
		}
	}

	free(contadorSeccion);
}

int contarSecciones(Producto *productos, int num_productos, int **seccionesEstante) {
	int *tempSecciones = malloc(num_productos * sizeof(int));
	int num_secciones = 0;

	for (int i = 0; i < num_productos; i++) {
		int existe = 0;
	    for (int j = 0; j < num_secciones; j++) {
	    	if (tempSecciones[j] == productos[i].codSeccion) {
	    		existe = 1;
	            break;
	        }
	    }
	    if (!existe) {
	        tempSecciones[num_secciones++] = productos[i].codSeccion;
	    }
	}

	*seccionesEstante = malloc(num_secciones * sizeof(int));
	memcpy(*seccionesEstante, tempSecciones, num_secciones * sizeof(int));
	free(tempSecciones);

	return num_secciones;
}


void mostrarProductosEstante(Producto **productosEstante, int *seccionesEstante, int num_secciones) {
	for(int i = 0; i < num_secciones; i++) {
		printf("Seccion: %d", seccionesEstante[i]);
		int j = 0;
		while (productosEstante[i][j].idProd != 0) {
			printf("  ID: %d, Nombre: %s, Precio: %.2f\n", productosEstante[i][j].idProd, productosEstante[i][j].nombreProd, productosEstante[i][j].precio);
			j++;
		}
	}
}

void imprimirProducto(Producto p) {
    printf("ID: %d, Nombre: %s, Precio: %.2f, Sección: %d, Proveedor: %d\n",
           p.idProd, p.nombreProd, p.precio, p.codSeccion, p.codProveedor);
}

void liberarMemoria(Producto **productosEstante, int num_secciones) {
	for(int i = 0; i < num_secciones; i++) {
		free(productosEstante[i]);
	}
	free(productosEstante);
}














