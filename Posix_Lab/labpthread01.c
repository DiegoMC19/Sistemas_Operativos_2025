/****************************************************************
* Pontificia Universidad Javeriana
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Profesor: J Corredor 
* Programa: Creación de hilos independientes
* Descripción: Crea dos hilos que ejecutan la misma función
*              con diferentes mensajes como argumento.
****************************************************************/

#include <stdio.h>  // printf: impresión en consola
#include <stdlib.h> // exit: terminación del programa
#include <string.h>  // Manipulación de strings
#include <unistd.h>  // Llamadas al sistema POSIX
#include <fcntl.h>  // Control de archivos
#include <sys/stat.h> // Información de archivos
#include <sys/types.h> // Tipos de datos del sistema
#include <pthread.h>  // Manejo de hilos POSIX

//Función que imprime un mensaje pasado como argumento 
void *print_message_function( void *ptr ) {
    char *message;
    message = (char *) ptr;  // Convierte el puntero void* a char*
    printf("%s \n", message); // Imprime el mensaje
}

void main() {
    pthread_t thread1, thread2; // Identificadores de los dos hilos
    char *message1 = "Thread 1"; // Mensaje para el primer hilo
    char *message2 = "Thread 2"; // Mensaje para el segundo hilo
    int  iret1, iret2;           // Variables para capturar códigos de retorno

    /* Crea hilos independientes, cada uno ejecutará la función */

    
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1); // Crea el primer hilo
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2); // Crea el segundo hilo

    // Imprime los códigos de retorno de pthread_create
    // Si es 0: hilo creado exitosamente, si no: hubo un error
    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);
    exit(0); // Termina el programa
}

