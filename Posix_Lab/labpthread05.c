/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Materia Sistemas Operativos
 * Docente: Corredor, PuD
 * Autor: Diego Mendoza
 * Fecha: 30/10/2025
 * Tema: Posix para la creación de hilos concurrentes:
 * Descripción:
 * Suponga que x*x es altamente costoso en cálculo computacional. Se desea calcular la suma
 * de los cuadrados de una cantidad de números. Se replica paralelizar el cálculo de cada
 * cuadrado usando hilos.
 **********************************************************************************************/

#include <stdio.h> // printf: impresión en consola 
#include <stdlib.h> // Funciones estándar (atoi, malloc, etc)
#include <string.h> // Manipulación de strings
#include <stdint.h> // intptr_t: tipo entero del tamaño de un puntero1~// intptr_t: tipo entero del tamaño de un puntero
#include <pthread.h> // Manejo de hilos POSIX

int acumulado = 0; // Variable global que acumula la suma de cuadrados

// Función que calcula el cuadrado de un número y lo suma al acumulado
void *cuadrado(void *x) {
    int num = (intptr_t)x;  // Convierte el puntero void* a entero
    acumulado += num * num; // Suma el cuadrado al acumulado global
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t hilos[20]; // Array para almacenar hasta 20 hilos
    
// Primer ciclo: Crea 20 hilos, cada uno calcula el cuadrado de (i+1)
    for(int i=0; i<20; i++) {
        // Crea un hilo que ejecuta la función cuadrado
        // Se pasa (i+1) como argumento convertido a puntero void*
        pthread_create(&hilos[i], NULL, cuadrado, (void *)(intptr_t)(i+1));
    }
    
    // Segundo ciclo: Espera a que todos los hilos terminen
    for(int i=0; i<20; i++) {
        void *resultado;
        pthread_join(hilos[i], &resultado); // Espera por el hilo i
    }
    
    // Imprime el resultado final: suma de cuadrados de 1 a 20
    printf("Valor suma cuadrados = %d\n", acumulado);
    
    return 0;
}
