/******************************************************************************
 * Nombres:     Diego Alejandro Mendoza Cruz
 *              Juan Diego Ariza Lopez
 * Materia:     Sistemas Operativos
 * Profesor:    John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha:       Noviembre/2025
 * Tema:        Proyecto 2025-30: Sistema de Reservas de Parque
 * Taller:      Simulación de Sistema de Reservas
 * Descripción: Implementación de funciones utilitarias compartidas entre
 *              Controlador y Agentes. Incluye funciones para manejo de
 *              errores, validación de parámetros y operaciones comunes
 *              del sistema de reservas.
 *****************************************************************************/


#include "Common.h"  // Definiciones compartidas (estructuras, constantes)
#include <stdarg.h>  // Manejo de argumentos variables (va_list, va_start, etc.)

// Función para imprimir errores y salir
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Función para logging con timestamp
void log_mensaje(const char *formato, ...) {
    // Obtener hora actual del sistema
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);
    
    // Imprimir timestamp en formato [HH:MM:SS]
    printf("[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
    
    // Procesar argumentos variables (como printf)
    va_list args;
    va_start(args, formato);
    vprintf(formato, args);
    va_end(args);
    
    printf("\n");
    // Forzar salida inmediata del buffer
    fflush(stdout);
}
