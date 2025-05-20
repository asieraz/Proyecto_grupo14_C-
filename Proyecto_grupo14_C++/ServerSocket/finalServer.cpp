/*
 * server.cpp
 *
 *  Created on: 20 may 2025
 *      Author: j.fernandezde
 */

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "finalServer.h"

extern "C" {
    #include "../includes/GestorDB.h"
	#include "../libs/sqlite3.h"
	#include "../includes/config.h"
}


#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

std::string mostrar_menu() {
    return "\n=== MENÚ ===\n"
           "1. Menú de proveedores\n"
           "2. Menú de clientes\n"
           "3. Salir\n"
           "Seleccione una opción: ";
}

void serverMain(sqlite3 *db) {

    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientLen = sizeof(clientAddr);
    char buffer[1024];
    int bytesRead;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup falló.\n";
        return;
    }

    // Crear socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creando el socket.\n";
        WSACleanup();
        return;
    }

    // Configurar dirección
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Asociar socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind falló.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Escuchar
    listen(serverSocket, 3);
    std::cout << "Servidor esperando conexiones en el puerto " << PORT << "...\n";

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error aceptando conexión.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Cliente conectado.\n";

    while (true) {
        std::string menu = mostrar_menu();
        send(clientSocket, menu.c_str(), menu.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cout << "Cliente desconectado.\n";
            break;
        }

        std::string opcion(buffer);
        opcion.erase(opcion.find_last_not_of(" \r\n") + 1);  // limpiar espacios y saltos

        std::string respuesta;



        if (opcion == "1") {
            mostrarProductos(db);
        } else if (opcion == "2") {
        printf("sin definir.\n");
        } else if (opcion == "3") {
            respuesta = "Hasta luego!\n";
            send(clientSocket, respuesta.c_str(), respuesta.size(), 0);
            break;
        } else {
            respuesta = "Opción no válida.\n";
        }

        send(clientSocket, respuesta.c_str(), respuesta.size(), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return;
}




