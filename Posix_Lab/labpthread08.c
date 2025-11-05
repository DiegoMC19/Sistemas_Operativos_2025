/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Materia Sistemas Operativos
 * Docente: Corredor, PuD
 * Autor: Diego Mendoza
 * Fecha: 30/10/2025
 * Tema: Hilos con API POSIX
 * Descripción:
 * 	Se tiene 2 funciones para asignar y reportar
 *	en función de la llamada desde los hilos
 *      creados, desde el proceso principal (main)
 *     
 *     Este código soluciona el problema de la condición de carrera 
 **********************************************************************************************/

#include <stdio.h> // printf: impresión en consola
#include <stdlib.h> // Funciones estándar
#include <pthread.h> // Manejo de hilos POSIX
#include <stdbool.h> // Tipo de dato bool (true/false)

pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER; // Variable de condición para sincronización
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger región crítica
int valor = 100;   // Valor compartido entre hilos
bool notificar = false; // Bandera de notificación

// Función reportar: espera notificación y luego imprime el valor
void *reportar(void *nousado) {
    /**/
    pthread_mutex_lock(&mutex); // Adquiere el mutex
    while(!notificar) { // Mientras no haya notificación
        pthread_cond_wait(&var_cond, &mutex); // Espera señal (libera y readquiere mutex automáticamente)
    }
    
    printf("El valor es: %d\n", valor); // Imprime el valor (puede ser incorrecto por CC)
    
    pthread_mutex_unlock(&mutex); // Libera el mutex
    
    return NULL; /*Nada que retornar: se previene la advertencia <warning>*/
}

// Función asignar: modifica el valor y notifica
void *asignar(void *nousado) {
    valor = 20; // Asigna nuevo valor (FUERA del mutex - CONDICIÓN DE CARRERA)
    
    pthread_mutex_lock(&mutex); // Adquiere el mutex
    notificar = true;           // Activa la bandera de notificación
    pthread_cond_signal(&var_cond); // Envía señal al hilo en espera
    pthread_mutex_unlock(&mutex); // Libera el mutex
    
    return NULL; /*Nada que retornar: se previene la advertencia <warning>*/
}

int main(int argc, char *argv[]) {
    pthread_t reporte, asigne; // Identificadores de los dos hilos
    
    // Crea los hilos
    pthread_create(&reporte, NULL, reportar, NULL); // Hilo que espera y reporta
    pthread_create(&asigne, NULL, asignar, NULL); // Hilo que asigna y notifica
    
    void *nousado; // Variable para capturar valor de retorno (no usado)
    
    // Espera a que ambos hilos terminen
    pthread_join(reporte, &nousado); // Espera al hilo reporte
    pthread_join(asigne, &nousado); // Espera al hilo asigne
    
    return 0;
}

