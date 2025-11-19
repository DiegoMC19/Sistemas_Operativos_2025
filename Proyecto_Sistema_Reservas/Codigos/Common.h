/******************************************************************************
 * Nombres:     Diego Alejandro Mendoza Cruz
 *              Juan Diego Ariza Lopez
 * Materia:     Sistemas Operativos
 * Profesor:    John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha:       Noviembre/2025
 * Tema:        Proyecto 2025-30: Sistema de Reservas de Parque
 * Taller:      Simulación de Sistema de Reservas
 * Descripción: Header con estructuras de datos, constantes y declaraciones
 *              de funciones compartidas entre el Controlador y los Agentes.
 *              Define los tipos de mensajes, estructuras para solicitudes,
 *              respuestas y configuración del sistema.
 *****************************************************************************/


#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>     // Entrada/salida estándar (printf, fprintf, FILE, etc.)
#include <stdlib.h>    // Funciones generales (exit, malloc, free, atoi, etc.)
#include <string.h>    // Manipulación de cadenas (strcpy, strcmp, strlen, etc.)
#include <unistd.h>    // API POSIX (read, write, close, sleep, getpid, etc.)
#include <fcntl.h>     // Control de archivos (open, O_RDONLY, O_WRONLY, etc.)
#include <sys/stat.h>  // Información de archivos (mkfifo, permisos, etc.)
#include <sys/types.h> // Tipos de datos del sistema (pid_t, ssize_t, etc.)
#include <signal.h>    // Manejo de señales (SIGALRM, signal, alarm, etc.)
#include <pthread.h>   // Hilos POSIX (pthread_create, pthread_mutex, etc.)
#include <time.h>      // Funciones de tiempo (time, localtime, struct tm, etc.)
#include <errno.h>     // Códigos de error (errno, perror, etc.)

// Constantes
#define MAX_NOMBRE 100     // Tamaño máximo para nombres 
#define MAX_LINEA 256      // Tamaño máximo para líneas de texto
#define MAX_AGENTES 50     // Número máximo de agentes/familias simultáneos
#define HORA_MIN 7         // Hora de apertura del parque (7:00 AM)
#define HORA_MAX 19        // Hora de cierre del parque (7:00 PM)
#define DURACION_RESERVA 2 // Duración de cada reserva en horas

// Tipos de mensaje
typedef enum {
    MSG_REGISTRO,
    MSG_SOLICITUD,
    MSG_RESPUESTA,
    MSG_TERMINAR
} TipoMensaje;

// Tipos de respuesta
typedef enum {
    RESP_OK,
    RESP_REPROGRAMADA,
    RESP_NEGADA_EXTEMPORANEA,
    RESP_NEGADA_SIN_CUPO,
    RESP_NEGADA_EXCEDE_AFORO
} TipoRespuesta;

// Estructura para registro inicial del agente
typedef struct {
    TipoMensaje tipo;
    char nombre_agente[MAX_NOMBRE];
    char pipe_respuesta[MAX_NOMBRE];
    int hora_actual;
} MensajeRegistro;

// Estructura para solicitud de reserva
typedef struct {
    TipoMensaje tipo;
    char nombre_agente[MAX_NOMBRE];
    char nombre_familia[MAX_NOMBRE];
    int hora_solicitada;
    int num_personas;
} MensajeSolicitud;

// Estructura para respuesta del controlador
typedef struct {
    TipoRespuesta tipo_respuesta;
    int hora_asignada;
    int num_personas;
    char mensaje[MAX_LINEA];
} MensajeRespuesta;

// Estructura para almacenar información de agentes registrados
typedef struct {
    char nombre[MAX_NOMBRE];
    char pipe_respuesta[MAX_NOMBRE];
    int activo;
} InfoAgente;

// Estructura para reservas por hora
typedef struct {
    int personas_reservadas;
    char familias[MAX_AGENTES][MAX_NOMBRE];
    int num_familias;
    int personas_por_familia[MAX_AGENTES];
    int hora_entrada[MAX_AGENTES];
} ReservaPorHora;

// Prototipos de funciones auxiliares
void error_exit(const char *msg);
void log_mensaje(const char *formato, ...);

#endif // COMMON_H
