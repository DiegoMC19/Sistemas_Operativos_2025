/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autores: Juan Diego Ariza y Diego Mendoza
 * Docente: J Corredor
 * Materia: Sistemas Operativos
 * Taller 03 - Actividad 2: Sincronización con Threads, Mutex y Variables de Condición
 * 
 * Descripción:
 * Este programa implementa un sistema de productores-spooler usando hilos POSIX.
 * Múltiples hilos productores generan mensajes que son impresos por un hilo spooler.
 *********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFERS 5  // Tamaño del buffer circular

// Buffer circular de cadenas
char buf[MAX_BUFFERS][100];
int buffer_index;         // Índice donde el productor escribe
int buffer_print_index;   // Índice donde el spooler lee

// Primitivas de sincronización
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;    // Señal: hay buffers disponibles
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER;  // Señal: hay líneas para imprimir

// Variables de control
int buffers_available = MAX_BUFFERS;  // Cantidad de buffers vacíos
int lines_to_print = 0;               // Cantidad de líneas listas para imprimir

// Prototipos de funciones
void *producer(void *arg);
void *spooler(void *arg);

int main(int argc, char **argv) {
    pthread_t tid_producer[10], tid_spooler;
    int i, r;

    printf("=== SISTEMA PRODUCTOR-SPOOLER INICIADO ===\n");
    printf("Buffers disponibles: %d\n", MAX_BUFFERS);
    printf("Número de productores: 10\n\n");

    // Inicializar índices del buffer
    buffer_index = buffer_print_index = 0;

    // Crear hilo spooler (impresora)
    if ((r = pthread_create(&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf(stderr, "Error creando spooler = %d (%s)\n", r, strerror(r));
        exit(1);
    }

    // Crear 10 hilos productores
    int thread_no[10];
    for (i = 0; i < 10; i++) {
        thread_no[i] = i;
        if ((r = pthread_create(&tid_producer[i], NULL, producer, (void *)&thread_no[i])) != 0) {
            fprintf(stderr, "Error creando productor %d = %d (%s)\n", i, r, strerror(r));
            exit(1);
        }
    }

    // Esperar a que todos los productores terminen
    for (i = 0; i < 10; i++) {
        if ((r = pthread_join(tid_producer[i], NULL)) != 0) {
            fprintf(stderr, "Error en join productor %d = %d (%s)\n", i, r, strerror(r));
            exit(1);
        }
    }

    printf("\n--- Todos los productores han terminado ---\n");

    // Esperar a que el spooler imprima todas las líneas pendientes
    while (lines_to_print > 0) {
        sleep(1);
    }

    printf("--- Todas las líneas han sido impresas ---\n");

    // Cancelar el hilo spooler (que está en un ciclo infinito)
    if ((r = pthread_cancel(tid_spooler)) != 0) {
        fprintf(stderr, "Error cancelando spooler = %d (%s)\n", r, strerror(r));
        exit(1);
    }

    // Esperar a que el spooler termine
    pthread_join(tid_spooler, NULL);

    printf("\n=== SISTEMA FINALIZADO CORRECTAMENTE ===\n");

    exit(0);
}

/**
 * Función del hilo productor
 * Cada productor genera 10 mensajes y los coloca en el buffer compartido
 */
void *producer(void *arg) {
    int i, r;
    int my_id = *((int *)arg);
    int count = 0;

    for (i = 0; i < 10; i++) {
        // Bloquear el mutex para acceder al buffer compartido
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error en mutex_lock = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        // Esperar mientras no haya buffers disponibles
        while (!buffers_available) {
            pthread_cond_wait(&buf_cond, &buf_mutex);
        }

        // Sección crítica: escribir en el buffer
        int j = buffer_index;
        buffer_index++;
        if (buffer_index == MAX_BUFFERS) {
            buffer_index = 0;  // Buffer circular
        }
        buffers_available--;

        // Generar el mensaje
        sprintf(buf[j], "Thread %d: mensaje %d\n", my_id, ++count);
        lines_to_print++;

        // Señalar al spooler que hay una nueva línea para imprimir
        pthread_cond_signal(&spool_cond);

        // Liberar el mutex
        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error en mutex_unlock = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        // Simular tiempo de producción
        sleep(1);
    }

    printf("[Productor %d terminó]\n", my_id);
    return NULL;
}

/**
 * Función del hilo spooler
 * Lee mensajes del buffer e los imprime continuamente
 */
void *spooler(void *arg) {
    int r;

    while (1) {
        // Bloquear el mutex para acceder al buffer compartido
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error en mutex_lock = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        // Esperar mientras no haya líneas para imprimir
        while (!lines_to_print) {
            pthread_cond_wait(&spool_cond, &buf_mutex);
        }

        // Sección crítica: leer e imprimir del buffer
        printf("%s", buf[buffer_print_index]);
        lines_to_print--;

        buffer_print_index++;
        if (buffer_print_index == MAX_BUFFERS) {
            buffer_print_index = 0;  // Buffer circular
        }

        buffers_available++;

        // Señalar a los productores que hay un buffer disponible
        pthread_cond_signal(&buf_cond);

        // Liberar el mutex
        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) {
            fprintf(stderr, "Error en mutex_unlock = %d (%s)\n", r, strerror(r));
            exit(1);
        }
    }

    return NULL;
}
