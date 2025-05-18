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

void run_server() {
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
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        printf("Command received: %s \n", recvBuff);
        fflush(stdout);

        if (strcmp(recvBuff, "SUMAR") == 0) {
            int suma = 0;
            do {
                memset(recvBuff, 0, sizeof(recvBuff));
                recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
                if (strcmp(recvBuff, "SUMAR-END") != 0) {
                    int n = atoi(recvBuff);
                    suma += n;
                }
            } while (strcmp(recvBuff, "SUMAR-END") != 0);
            sprintf(sendBuff, "%d", suma);
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            printf("Response sent: %s \n", sendBuff);
            fflush(stdout);
        } else if (strcmp(recvBuff, "RAIZ") == 0) {
            memset(recvBuff, 0, sizeof(recvBuff));
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            int n = atoi(recvBuff);
            float raiz = sqrt(n);

            memset(recvBuff, 0, sizeof(recvBuff));
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            // "RAIZ-END" is just for confirmation

            sprintf(sendBuff, "%f", raiz);
            send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            printf("Response sent: %s \n", sendBuff);
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
