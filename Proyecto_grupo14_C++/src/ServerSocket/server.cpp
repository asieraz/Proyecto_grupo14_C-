/*
 * server.cpp
 *
 *  Created on: 20 may 2025
 *      Author: a.azcorra
 */


#include "../../includes/operaciones.h"
#include "../../includes/finalInitDatabase.h"
#include "../../includes/anadirDatosDB.h"
#include "../../includes/GestorDB.h"
#include "../../libs/sqlite3.h"
#include "../../includes/interfaz.h"
#include "../../includes/estructuras.h"
#include "../../includes/config.h"
#include "../../libs/sqlite3.h"
#include "../../includes/server.h"
#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

void run_server(sqlite3 *db) {
    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];

    printf("\nInitialising Winsock...\n");
    fflush(stdout);
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    printf("Initialised.\n");
    fflush(stdout);

    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

    printf("Socket created.\n");
    fflush(stdout);

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }

    printf("Bind done.\n");
    fflush(stdout);

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }

    printf("Waiting for incoming connections...\n");
    fflush(stdout);
    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
    if (comm_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }
    printf("Incoming connection from: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    closesocket(conn_socket);

    printf("Waiting for incoming commands from client... \n");
    fflush(stdout);

    do {
        memset(recvBuff, 0, sizeof(recvBuff)); //vacia el recvBuff para asegurarse de que este vacio
        recv(comm_socket, recvBuff, sizeof(recvBuff), 0); //recibe el mensaje (normalmente el comando que quiere que el server ejecute) del cliente y lo guarda en recvBuff
        printf("Command received: %s \n", recvBuff); //muestra el mensaje que ha mandado el cliente
        fflush(stdout);

        if (strcmp(recvBuff, "SUMAR") == 0) { //si el recvBuff == "SUMAR"
            int suma = 0;
            do {
                memset(recvBuff, 0, sizeof(recvBuff)); //vacia recvBuff
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);//el cliente va a mandar numeros hasta que mande un "SUMAR-END" que sera la señal de que ya ha terminado
                if (strcmp(recvBuff, "SUMAR-END") != 0) {//si el recvBuff no es igual a "SUMAR-END" suma el numero recivido
                    int n = atoi(recvBuff); //combierte el numero recivido a un entero
                    suma += n;
                }
            } while (strcmp(recvBuff, "SUMAR-END") != 0); //repite el proceso hasta que recvBuff = "SUMAR-END"
            sprintf(sendBuff, "%d", suma); //transforma el resultado en un string y lo guarda en sendBuff
            send(comm_socket, sendBuff, sizeof(sendBuff), 0); //envia el resultado al cliente
            printf("Response sent: %s \n", sendBuff); //printea la respuesta que se le ha dado al cliente
            fflush(stdout);

        } else if (strcmp(recvBuff, "MOSTRAR") == 0) {
            memset(recvBuff, 0, sizeof(recvBuff));
            //------------------------------------
        	sqlite3_stmt *stmt;

        	char sql[] = "select id_Producto, nombre, precio, id_Proveedor, cod_Seccion, stock from producto";

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
        	    int stock = sqlite3_column_int(stmt, 5);

        	   if(!idProd == 0) {
        		   sprintf(sendBuff, "Producto: %d, Nombre: %s, Precio: %.2f, Proveedor: %d, Seccion: %d, stock: %d\n", idProd, nombreProd, precio, codProveedor, codSeccion, stock);
        		   send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        	    }

        	}
        	strcpy(sendBuff, "MOSTRAR-END");// la señal de que ya has terminado de enviar datos
        	send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        	sqlite3_finalize(stmt);
        	//------------------------------------

            printf("Productos mostrados \n");
            fflush(stdout);
        } else if (strcmp(recvBuff, "IP") == 0) {
            memset(recvBuff, 0, sizeof(recvBuff));
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            // "IP-END" confirmation

            strcpy(sendBuff, inet_ntoa(server.sin_addr));
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            printf("Response sent: %s \n", sendBuff);
            fflush(stdout);
        } else if (strcmp(recvBuff, "Anadir stock") == 0) {
        	memset(recvBuff, 0, sizeof(recvBuff));

        	// Recibir ID del producto
        	recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	int idProducto = atoi(recvBuff);
        	printf("ID producto recibido: %d\n", idProducto);

        	// Recibir cantidad a añadir
        	memset(recvBuff, 0, sizeof(recvBuff));
        	recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	int cantidad = atoi(recvBuff);
        	printf("Cantidad a añadir: %d\n", cantidad);

        	// Obtener el stock actual
        	sqlite3_stmt *stmt;
        	char selectSQL[] = "SELECT stock FROM producto WHERE id_Producto = ?";
        	int result = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
        	if (result != SQLITE_OK) {
        		printf("Error preparando SELECT: %s\n", sqlite3_errmsg(db));
        		return;
        	}

        	sqlite3_bind_int(stmt, 1, idProducto); //Sirve para poner el valor en la interrogacion de la consulta

        	int stockActual = -1;
        	if (sqlite3_step(stmt) == SQLITE_ROW) {
        		stockActual = sqlite3_column_int(stmt, 0);
        	}
        	sqlite3_finalize(stmt);

        	if (stockActual == -1) {
        		sprintf(sendBuff, "❌ Producto con ID %d no encontrado.\n", idProducto);
        		send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        		return;
        	}

        	int nuevoStock = stockActual + cantidad;

        	// Actualizar en la BD
        	char updateSQL[] = "UPDATE producto SET stock = ? WHERE id_Producto = ?";
        	result = sqlite3_prepare_v2(db, updateSQL, -1, &stmt, NULL);
        	if (result != SQLITE_OK) {
        		printf("Error preparando UPDATE: %s\n", sqlite3_errmsg(db));
        		return;
        	}

        	sqlite3_bind_int(stmt, 1, nuevoStock);
        	sqlite3_bind_int(stmt, 2, idProducto);

        	if (sqlite3_step(stmt) != SQLITE_DONE) {
        		printf("Error al ejecutar UPDATE: %s\n", sqlite3_errmsg(db));
        		sqlite3_finalize(stmt);
        		return;
        	}

        	sqlite3_finalize(stmt);

        	// Confirmar al cliente
        	sprintf(sendBuff, "Stock del producto %d actualizado a %d unidades.\n", idProducto, nuevoStock);
        	send(comm_socket, sendBuff, sizeof(sendBuff), 0);

        	printf("Stock actualizado correctamente.\n");
        	fflush(stdout);

        }

        else if(strcmp(recvBuff, "REALIZAR COMPRA") == 0) {
        		char* fecha = "2025-05-24";
        	    char dniCliente[32];

        	    // PEDIR DNI
        	    send(comm_socket, "Introduce tu DNI:", 512, 0);
        	    memset(recvBuff, 0, sizeof(recvBuff));
        	    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	    strncpy(dniCliente, recvBuff, sizeof(dniCliente));
        	    dniCliente[sizeof(dniCliente)-1] = '\0';

        	    // COMPROBAR QUE EXISTE EL CLIENTE
        	    sqlite3_stmt *stmt;
        	    const char *checkCliente = "SELECT 1 FROM cliente WHERE DNI = ?";
        	    sqlite3_prepare_v2(db, checkCliente, -1, &stmt, NULL);
        	    sqlite3_bind_text(stmt, 1, dniCliente, -1, SQLITE_STATIC);

        	    int existeCliente = (sqlite3_step(stmt) == SQLITE_ROW);
        	    sqlite3_finalize(stmt);

        	    if (!existeCliente) {
        	        send(comm_socket, "Cliente no encontrado. Compra cancelada.\n", 512, 0);
        	        return;
        	    }

        	    // INSERTAR COMPRA CON DNI VÁLIDO
        	    const char *insertCompraSQL = "INSERT INTO compra (fecha, DNI) VALUES (?, ?);";
        	    int result = sqlite3_prepare_v2(db, insertCompraSQL, -1, &stmt, NULL);
        	    if (result != SQLITE_OK) {
        	        printf("Error preparando INSERT de compra: %s\n", sqlite3_errmsg(db));
        	        return;
        	    }

        	    sqlite3_bind_text(stmt, 1, fecha, -1, SQLITE_STATIC);
        	    sqlite3_bind_text(stmt, 2, dniCliente, -1, SQLITE_STATIC);
        	    sqlite3_step(stmt);
        	    sqlite3_finalize(stmt);

        	    int idCompra = (int)sqlite3_last_insert_rowid(db);

        	    // BUCLE DE PRODUCTOS
        	    while (1) {
        	        send(comm_socket, "ID producto:", 512, 0);
        	        memset(recvBuff, 0, sizeof(recvBuff));
        	        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	        int idProd = atoi(recvBuff);

        	        send(comm_socket, "Cantidad:", 512, 0);
        	        memset(recvBuff, 0, sizeof(recvBuff));
        	        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	        int cantidad = atoi(recvBuff);

        	        // CONSULTAR STOCK
        	        const char *selectSQL = "SELECT stock FROM producto WHERE id_Producto = ?";
        	        sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
        	        sqlite3_bind_int(stmt, 1, idProd);
        	        int stock = -1;

        	        if (sqlite3_step(stmt) == SQLITE_ROW) {
        	            stock = sqlite3_column_int(stmt, 0);
        	        }
        	        sqlite3_finalize(stmt);

        	        if (stock < cantidad || stock == -1) {
        	            send(comm_socket, "Stock insuficiente o producto no existe.\n", 512, 0);
        	        } else {
        	            // INSERTAR EN PRODUCTOENCOMPRA
        	            const char *insertProd = "INSERT INTO ProductoEnCompra (id_Compra, id_Producto, cantidad) VALUES (?, ?, ?);";
        	            sqlite3_prepare_v2(db, insertProd, -1, &stmt, NULL);
        	            sqlite3_bind_int(stmt, 1, idCompra);
        	            sqlite3_bind_int(stmt, 2, idProd);
        	            sqlite3_bind_int(stmt, 3, cantidad);
        	            sqlite3_step(stmt);
        	            sqlite3_finalize(stmt);

        	            // ACTUALIZAR STOCK
        	            const char *updateStock = "UPDATE producto SET stock = stock - ? WHERE id_Producto = ?";
        	            sqlite3_prepare_v2(db, updateStock, -1, &stmt, NULL);
        	            sqlite3_bind_int(stmt, 1, cantidad);
        	            sqlite3_bind_int(stmt, 2, idProd);
        	            sqlite3_step(stmt);
        	            sqlite3_finalize(stmt);

        	            send(comm_socket, "Producto anadido a la compra.\n", 512, 0);
        	        }

        	        // ¿QUIERE AÑADIR OTRO?
        	        send(comm_socket, "¿Desea anadir otro producto? (s/n):", 512, 0);
        	        memset(recvBuff, 0, sizeof(recvBuff));
        	        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

        	        if (recvBuff[0] == 'n' || recvBuff[0] == 'N') {
        	            break;
        	        }
        	    }

        	    send(comm_socket, "Compra finalizada.\n", 512, 0);
                }

        else if (strcmp(recvBuff, "BUSCADOR") == 0) {
            // Enviar menú
            strcpy(sendBuff,
                "Opciones de búsqueda:\n"
                "1. Buscar por nombre\n"
            	"2. Buscar por seccion\n"
            	"3. Buscar por id\n"
                "4. Buscar por precio (mayor o menor que X)\n"
                "5. Buscar por stock (mayor o menor que X)\n"
                "Selecciona opcion (1, 4, 5): ");
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);

            // Recibir opción
            memset(recvBuff, 0, sizeof(recvBuff));
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            int opcion = atoi(recvBuff);

            sqlite3_stmt *stmt;

            if (opcion == 1) {
                // Buscar por nombre parcial
                send(comm_socket, "Introduce parte del nombre: ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                char likeQuery[128];
                snprintf(likeQuery, sizeof(likeQuery), "%%%s%%", recvBuff);

                const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE nombre LIKE ?;";
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                sqlite3_bind_text(stmt, 1, likeQuery, -1, SQLITE_STATIC);
            }

            else if (opcion == 2) {
                // Buscar por seccion
                send(comm_socket, "Introduce código de sección: ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                int codSeccion = atoi(recvBuff);

                const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE cod_Seccion = ?;";
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, codSeccion);
            }

            else if (opcion == 3) {
                // Buscar por ID de producto
                send(comm_socket, "Introduce ID del producto: ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                int idProd = atoi(recvBuff);

                const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE id_Producto = ?;";
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, idProd);
            }

            else if (opcion == 4) {
                // Buscar por precio
                send(comm_socket, "Buscar precio (mayor o menor): (mayor/menor): ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                int mayor = 0;
                if (strcmp(recvBuff, "mayor") == 0) {
                    mayor = 1;
                }

                send(comm_socket, "Introduce valor de precio: ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                float precio = atof(recvBuff);

                if (mayor == 1) {
                    const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE precio >= ?;";
                    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                    sqlite3_bind_double(stmt, 1, precio);
                } else {
                    const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE precio <= ?;";
                    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                    sqlite3_bind_double(stmt, 1, precio);
                }
            }

            else if (opcion == 5) {
                // Buscar por stock
                send(comm_socket, "Buscar stock (mayor o menor): (mayor/menor): ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                int mayor = 0;
                if (strcmp(recvBuff, "mayor") == 0) {
                    mayor = 1;
                }

                send(comm_socket, "Introduce valor de stock: ", 512, 0);
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                int stock = atoi(recvBuff);

                if (mayor == 1) {
                    const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE stock >= ?;";
                    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                    sqlite3_bind_int(stmt, 1, stock);
                } else {
                    const char *sql = "SELECT id_Producto, nombre, precio, stock FROM producto WHERE stock <= ?;";
                    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                    sqlite3_bind_int(stmt, 1, stock);
                }
            }

            // Mostrar resultados
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                const unsigned char *nombre = sqlite3_column_text(stmt, 1);
                double precio = sqlite3_column_double(stmt, 2);
                int stock = sqlite3_column_int(stmt, 3);

                sprintf(sendBuff, "ID: %d | %s | Precio: %.2f | Stock: %d\n", id, nombre, precio, stock);
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            }

            sqlite3_finalize(stmt);
            strcpy(sendBuff, "BUSCADOR-END");
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        }

        else if (strcmp(recvBuff, "COMPRAS CLIENTE") == 0) {
        	// Pedir DNI
        	    send(comm_socket, "Introduce tu DNI:", 512, 0);
        	    memset(recvBuff, 0, sizeof(recvBuff));
        	    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        	    char dni[32];
        	    strncpy(dni, recvBuff, sizeof(dni));
        	    dni[sizeof(dni)-1] = '\0';

        	    // Validar cliente
        	    sqlite3_stmt *stmt;
        	    const char *checkSQL = "SELECT 1 FROM cliente WHERE DNI = ?";
        	    sqlite3_prepare_v2(db, checkSQL, -1, &stmt, NULL);
        	    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);

        	    int exists = (sqlite3_step(stmt) == SQLITE_ROW);
        	    sqlite3_finalize(stmt);

        	    if (!exists) {
        	        strcpy(sendBuff, "Cliente no encontrado. Volviendo al menu.\nCOMPRAS-END");
        	        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        	        return;
        	    }

        	    // Buscar compras del cliente
        	    const char *compraSQL = "SELECT id_Compra, fecha FROM compra WHERE DNI = ?;";
        	    sqlite3_prepare_v2(db, compraSQL, -1, &stmt, NULL);
        	    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);

        	    while (sqlite3_step(stmt) == SQLITE_ROW) {
        	        int idCompra = sqlite3_column_int(stmt, 0);
        	        const unsigned char *fecha = sqlite3_column_text(stmt, 1);

        	        sprintf(sendBuff, "🧾 Compra #%d | Fecha: %s\n", idCompra, fecha);
        	        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

        	        sqlite3_stmt *stmtProd;
        	        const char *prodSQL =
        	            "SELECT producto.nombre, productoEnCompra.cantidad "
        	            "FROM productoEnCompra "
        	            "JOIN producto ON producto.id_Producto = productoEnCompra.id_Producto "
        	            "WHERE productoEnCompra.id_Compra = ?;";
        	        sqlite3_prepare_v2(db, prodSQL, -1, &stmtProd, NULL);
        	        sqlite3_bind_int(stmtProd, 1, idCompra);

        	        while (sqlite3_step(stmtProd) == SQLITE_ROW) {
        	            const unsigned char *nombre = sqlite3_column_text(stmtProd, 0);
        	            int cantidad = sqlite3_column_int(stmtProd, 1);
        	            sprintf(sendBuff, "   - %s (x%d)\n", nombre, cantidad);
        	            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        	        }
        	        sqlite3_finalize(stmtProd);
        	        send(comm_socket, "\n", sizeof(sendBuff), 0);
        	    }

        	    sqlite3_finalize(stmt);
        	    strcpy(sendBuff, "COMPRAS-END");
        	    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        }

        else if (strcmp(recvBuff, "EXIT") == 0) {
            break;
        }
    } while (1);

    closesocket(comm_socket);
    WSACleanup();

}
