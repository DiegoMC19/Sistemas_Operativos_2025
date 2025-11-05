/*************************************************************************
 * Nombre: Diego Mendoza
 * Materia: Sistemas Operativos
 * Profesor: John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha: 30/10/2025
 * Tema: Hilos con API POSIX
 * Descripción: Implementación de concurrencia mediante la creación
 *              de múltiples hilos que acceden y modifican un recurso
 *              compartido. Se utiliza mutex para garantizar exclusión
 *              mutua y evitar condiciones de carrera.
 *              
 *
 *************************************************************************/

#include <stdio.h> // printf: impresión en consola
#include <stdlib.h> // Funciones estándar
#include <pthread.h> // Manejo de hilos POSIX
#include <unistd.h> // sleep, usleep: funciones de suspensión

#define NTHREADS 10  // Número total de hilos a crear

int counter = 0; // Contador compartido entre todos los hilos


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger contador

// Función que ejecuta cada hilo
void* thread_function(void* arg)
{
   
    int i = *(int*)(arg); // Obtiene el número del hilo

   // Imprime información del hilo (ID del hilo y su pthread_self)
    printf("Hilo numero: %d | Hilo ID %ld\n", i, pthread_self());

   // Sección crítica: incrementa el contador de forma segura
    pthread_mutex_lock(&mutex1);// Adquiere el mutex
    counter++; // Incrementa contador compartido
    pthread_mutex_unlock(&mutex1); // Libera el mutex

   
    return NULL;
}


int main(void)
{
    pthread_t thread_id[NTHREADS]; // Array de identificadores de hilos
    int i, j;
    int thread_args[NTHREADS];    // Array de argumentos para cada hilo

    // Primer ciclo: Crea NTHREADS hilos
    for (i = 0; i < NTHREADS; i++)
    {
        thread_args[i] = i + 1;  // Asigna número de hilo (1 a 10)
        pthread_create(&thread_id[i], NULL, thread_function, &thread_args[i]); // Crea el hilo pasándole su número como argumento
    }

    // Segundo ciclo: Espera a que todos los hilos terminen
    for (j = 0; j < NTHREADS; j++)
    {
        pthread_join(thread_id[j], NULL); // Espera por cada hilo
    }

    // Imprime el valor final del contador
    printf("Contador final: %d\n", counter);

    return 0;
}
