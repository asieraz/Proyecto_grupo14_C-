/*
 * server.c
 *
 *  Created on: 17 may 2025
 *      Author: a.azcorra
 */

#include "../includes/operaciones.h"
#include "../includes/finalInitDatabase.h"
#include "../includes/anadirDatosDB.h"
#include "../includes/GestorDB.h"
#include "../libs/sqlite3.h"
#include "../includes/interfaz.h"
#include "../includes/estructuras.h"
#include "../includes/config.h"
#include "../libs/sqlite3.h"
#include "../includes/server.h"
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
        		   sprintf(sendBuff, "Producto: %d, Nombre: %s, Precio: %.2f, Proveedor: %d, Seccion: %d\n", idProd, nombreProd, precio, codProveedor, codSeccion);
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
        } else if (strcmp(recvBuff, "EXIT") == 0) {
            break;
        }
    } while (1);

    closesocket(comm_socket);
    WSACleanup();

}
