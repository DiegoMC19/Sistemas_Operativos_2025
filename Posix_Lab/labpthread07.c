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
 *     El laboratorio presenta CC, ya que de cada
 *     100 veces que se ejecuta, algunas veces el resultado no es preciso  
 **********************************************************************************************/

#include <stdio.h>  // printf: impresión en consola
#include <stdlib.h> // Funciones estándar
#include <pthread.h> // Manejo de hilos POSIX
#include <stdbool.h> // Tipo de dato bool (true/false)

pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER; // Variable de condición
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para sincronización
int valor = 100;   // Valor inicial compartido
bool notificar = false; // Bandera de notificación

// Función reportar: espera a ser notificada y luego imprime el valor
void *reportar(void *nousado){
    /*
    pthread_mutex_lock(&mutex);
    while(!notificar){
        pthread_cond_wait(&var_cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    */
    printf("El valor es: %d\n", valor);
    
    return NULL; /*Nada que retornar: se previene la advertencia <warning>*/
}

// Función asignar: modifica el valor y notifica al otro hilo
void *asignar(void *nousado){
    valor = 20; // Asigna nuevo valor (FUERA del mutex - condición de carrera)
    /*
    pthread_mutex_lock(&mutex);
    notificar = true;
    pthread_cond_signal(&var_cond);
    pthread_mutex_unlock(&mutex);
    */
    return NULL; /*Nada que retornar: se previene la advertencia <warning>*/
}

int main(int argc, char *argv[]){
    pthread_t reporte, asigne; // Identificadores de los hilos
    
    // Crea los dos hilos
    pthread_create(&reporte, NULL, reportar, NULL); // Hilo que reporta
    pthread_create(&asigne, NULL, asignar, NULL);   // Hilo que asigna
    
    void *nousado; // Variable para pthread_join
    
    // Espera a que ambos hilos terminen
    pthread_join(reporte, &nousado);
    pthread_join(asigne, &nousado);
    
    return 0;
}
