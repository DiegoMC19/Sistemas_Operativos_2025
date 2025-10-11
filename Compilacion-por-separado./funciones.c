#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

// Muestra el encabezado del programa
void printFunction() {
    printf("*****************************************\n");
    printf("*    Bienvenido al programa modular C   *\n");
    printf("*      Autor: Diego Mendoza             *\n");
    printf("*****************************************\n\n");
}

// Calcula el índice de masa corporal (IMC) = masa / (altura^2)
float imcFuncion(float masa, float altura) {
    return masa / (altura * altura);
}

// Llena una matriz cuadrada de tamaño NxN con números aleatorios entre 0 y 9
void llenarMatriz(float* matriz, int N) {
    for (int i = 0; i < N * N; i++) {
        matriz[i] = rand() % 10; // Valor aleatorio entre 0 y 9
    }
}

// Imprime una matriz cuadrada de tamaño NxN
void impMatrix(float* matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.2f ", matriz[i * N + j]);
        }
        printf("\n");
    }
}

// Realiza la multiplicación de dos matrices cuadradas A y B, el resultado se guarda en C
void matrixMult(float* A, float* B, float* C, int N) {
    for (int i = 0; i < N; i++) {           // Recorre filas de A
        for (int j = 0; j < N; j++) {       // Recorre columnas de B
            C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {   // Multiplica fila de A por columna de B
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}
