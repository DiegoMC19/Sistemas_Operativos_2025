#ifndef FUNCIONES_H
#define FUNCIONES_H

// Imprime información inicial del programa
void printFunction();

// Calcula el índice de masa corporal (IMC)
float imcFuncion(float masa, float altura);

// Llena una matriz NxN con valores aleatorios entre 0 y 9
void llenarMatriz(float* matriz, int N);

// Imprime una matriz NxN
void impMatrix(float* matriz, int N);

// Realiza la multiplicación de dos matrices cuadradas A x B = C
void matrixMult(float* A, float* B, float* C, int N);

#endif
