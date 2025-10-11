/***************************************************
*          Pontificia Universidad Javeriana        *
****************************************************
* Autor: Diego Mendoza                             *
* Fecha: 31 de julio 2025                          *
* Docente: J. Corredor                             *
* Objetivo: Punteros en C - Compilación modular    *
***************************************************/

#include <stdio.h>      // Librería estándar de entrada/salida
#include <stdlib.h>     // Librería para funciones de conversión y malloc
#include <time.h>       // Librería para manejo de tiempo (semilla aleatoria)
#include "funciones.h"  // Cabecera con las funciones propias

int main(int argc, char* argv[]){
    // Verifica que se pasen los argumentos necesarios al ejecutar
    if(argc < 4){
        printf("Uso: ./ejecutable altura masa N\n");
        return 1;
    }

    // Conversión de los argumentos de entrada
    float altura = atof(argv[1]); // Altura en metros
    float masa = atof(argv[2]);   // Masa en kilogramos
    int N = atoi(argv[3]);        // Tamaño de la matriz NxN

    srand(time(NULL)); // Inicializa la semilla para generar números aleatorios

    printFunction(); // Muestra mensaje introductorio o informativo

    // Calcula el Índice de Masa Corporal (IMC)
    float imc = imcFuncion(masa, altura);
    printf("El índice de masa corporal es: %.2f\n\n", imc);

    // Reserva memoria dinámica para 3 matrices NxN
    float* m1 = malloc(N * N * sizeof(float));
    float* m2 = malloc(N * N * sizeof(float));
    float* m3 = malloc(N * N * sizeof(float));

    // Llena las matrices A y B con números aleatorios
    llenarMatriz(m1, N);
    llenarMatriz(m2, N);

    // Imprime las matrices A y B
    printf("Matriz A:\n");
    impMatrix(m1, N);
    printf("\nMatriz B:\n");
    impMatrix(m2, N);

    // Multiplica las matrices A y B, y guarda el resultado en m3
    matrixMult(m1, m2, m3, N);

    // Imprime la matriz resultante
    printf("\nResultado de la multiplicación A x B:\n");
    impMatrix(m3, N);

    // Libera la memoria reservada
    free(m1);
    free(m2);
    free(m3);

    return 0;
}
