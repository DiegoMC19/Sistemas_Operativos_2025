/************************************************************
*                    Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: OpenMP - Cálculo de sumatoria con función seno
* Descripción:
*   Programa que usa paralelización con OpenMP y la cláusula
*   reduction para calcular una sumatoria basada en la función seno.
*   Cada hilo calcula una parte de la sumatoria y luego se combinan
*   los resultados parciales para obtener el total.
************************************************************/

#include <stdio.h>   // Librería para entrada/salida
#include <stdlib.h>  // Librería estándar de C
#include <omp.h>     // Librería para paralelismo con OpenMP
#include <math.h>    // Librería matemática (para usar sin)

// Macro que devuelve el mínimo entre dos valores
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Función f: calcula la suma de senos desde "start" hasta "finish"
double f(int i) {
    double j, start = i * (i + 1) / 2, finish = start + i;
    int j_int;
    double return_val = 0;

    // Recorre el rango y acumula el seno de cada valor
    for (j_int = start; j_int < finish; j_int++)
        return_val += sin(j_int);

    return return_val;
}

// Función principal
int main() {
    double sum = 0;    // Acumulador para la sumatoria total
    int i, n = 30000;  // Límite de iteraciones

    // Región paralela: se crean múltiples hilos
    #pragma omp parallel
    {
        // Obtiene el número de hilos disponibles
        int threads = omp_get_num_threads();

        // Solo el hilo maestro imprime la cantidad de hilos
        #pragma omp master
        printf("Número máximo de hilos del SO: %d\n", threads);

        // Bucle paralelo con reducción: cada hilo calcula parte de la suma
        #pragma omp for reduction(+: sum)
        for (i = 0; i < n; i++) {
            sum += f(i);
        }
    }

    // Muestra el resultado final
    printf("La sumatoria es: %.2f\n", sum);

    return 0;  // Fin del programa
}

