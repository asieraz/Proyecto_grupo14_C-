/*
 * logs.cpp
 *
 *  Created on: 30 may 2025
 *      Author: a.azcorra
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include <string.h>

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

void logComando(const std::string& tipo, const std::string& mensaje) {
    std::ofstream logFile("servidor_logs.txt", std::ios::app); // modo append
    if (!logFile.is_open()) return;

    // Obtener tiempo actual
    std::time_t ahora = std::time(nullptr);
    std::tm* tiempoLocal = std::localtime(&ahora);
    std::ostringstream timestamp;
    timestamp << std::put_time(tiempoLocal, "[%Y-%m-%d %H:%M:%S]");

    logFile << timestamp.str() << " [" << tipo << "] " << mensaje << "\n";
    logFile.close();
}
