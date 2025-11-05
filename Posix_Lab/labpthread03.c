/***************************************************************
* Pontificia Universidad Javeriana
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Profesor: J Corredor 
* Programa: Sincronización con mutex y variables de condición
* Descripción: Dos hilos  incrementan un contador compartido.
*              Uno espera hasta que el contador esté en un rango,
* 	       el otro señaliza cuando sale del rango.
****************************************************************/

#include <stdio.h> // printf: impresión en consola
#include <stdlib.h> // Funciones estándar
#include <string.h> // Manipulación de strings
#include <unistd.h> // Llamadas al sistema POSIX
#include <fcntl.h> // Control de archivos
#include <sys/stat.h> // Información de archivos
#include <sys/types.h> // Tipos de datos del sistema
#include <pthread.h>  // Manejo de hilos POSIX


// Inicialización de mutex y condición
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;      // Protege variable count
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;  // Protege variable de condición
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;     // Variable de condición

// Variables globales y constantes
int count = 0;            // Contador compartido entre hilos
#define COUNT_DONE 15     // Valor final del contador
#define COUNT_HALT1 4     // Límite inferior del rango de pausa
#define COUNT_HALT2 11    // Límite superior del rango de pausa

// Función del primer hilo: incrementa el contador y espera si está en el rango
void *count01(void *arg) {
    for (;;) { // Ciclo infinito
        pthread_mutex_lock(&condition_mutex);
        // Espera si count está entre HALT1 y HALT2
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2) {
            pthread_cond_wait(&condition_cond, &condition_mutex); // Libera mutex y espera señal
        }
        pthread_mutex_unlock(&condition_mutex);

        // Sección crítica: incrementa el contador
        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount01 == %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        // Condición de finalización
        if (count >= COUNT_DONE)
            return NULL;
    }
}

// Función del segundo hilo: incrementa el contador y señaliza cuando sale del rango
void *count02(void *arg) {
    for (;;) { // Ciclo infinito
        pthread_mutex_lock(&condition_mutex);
        // Señala cuando count está fuera del rango de pausa
        if (count < COUNT_HALT1 || count > COUNT_HALT2) {
            pthread_cond_signal(&condition_cond); // Despierta al hilo en espera 
        }
        pthread_mutex_unlock(&condition_mutex);

        // Sección crítica: incrementa el contador 
        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount02 == %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        // Condición de finalización
        if (count >= COUNT_DONE)
            return NULL;
    }
}


int main() {

    pthread_t thread1, thread2; // Identificadores de los hilos

    // Creación de los hilos
    pthread_create(&thread1, NULL, count01, NULL); // Hilo 1: count01
    pthread_create(&thread2, NULL, count02, NULL); // Hilo 2: count02

    // Espera a que terminen ambos hilos
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nFinal counter value: %d\n", count);
    return 0;
}

