/*********************************************************
* Pontificia Universidad Javeriana
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Profesor: J Corredor
* Tema: Hilos con API POSIX
* Variables de Condición y Exclusión Mutua
* Sincronización de salidas de cada hilo
*********************************************************/

#include <pthread.h> // Manejo de hilos POSIX
#include <stdio.h>   // printf, fprintf: impresión en consola
#include <string.h>  // strerror: conversión de códigos de error a texto

pthread_t tid[3];    // Arreglo de 3 identificadores de hilos
int counter;         // Contador compartido entre hilos
pthread_mutex_t lock; // Mutex para proteger la sección crítica

// Función que ejecutará cada hilo
void* compute(void *arg) { 
    unsigned long i = 0;
    
    pthread_mutex_lock(&lock);  // Adquiere el mutex (entra a sección crítica)
    counter += 1;               // Incrementa contador de forma segura
    printf("\n Job %d has started\n", counter);
    
    for(i = 0; i < 0xFFFFFF; i++); // Ciclo de trabajo simulado (cuenta hasta 0xFFFFFF)
    
    printf("\n Job %d has finished\n", counter);
    pthread_mutex_unlock(&lock); // Libera el mutex (sale de sección crítica)
    
    return NULL;
}

int main(void) {
    int i = 0;
    int error;
    
    // Inicializa el mutex
    // pthread_mutex_init retorna 0 si es exitoso
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    
    // Crea 3 hilos
    while(i < 3) {
        error = pthread_create(&(tid[i]), NULL, &compute, NULL);
        if (error != 0) { // Si hay error en la creación
            printf("\nThread can't be created : [%s]", strerror(error)); // strerror convierte el código de error en mensaje legible
        }
        i++;
    }
    
    // Espera a que todos los hilos terminen
    pthread_join(tid[0], NULL); // Espera al hilo 0
    pthread_join(tid[1], NULL); // Espera al hilo 1
    pthread_join(tid[2], NULL); // Espera al hilo 2

    // Destruye el mutex (libera recursos)
    pthread_mutex_destroy(&lock);
    
    return 0;
}
