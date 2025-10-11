/**************************************************************************************
 * Fecha: 25 Septiembre 2025
 * Autor: Diego Alejandro Mendoza Cruz
 * Materia: Sistemas Operativos
 * Tema: Concurrencia
 * Objetivo: Hacer una metodología para la implementación de la multiplicación de matrices.
 * La idea principal, es construir paso a paso la implementación para hacer uso de la
 * biblioteca PTHREAD. Se implementa el Algoritmo Clásico de multiplicación de matrices, para
 * matrices cuadradas, es decir, la dimensión de filas es igual a la de columnas.
 * A continuación se describen los paso
 *  - Reserva de memoria
 *      - Creación de punteros para matrices del tipo doble
 *      - Asignación de memoria
 *      - Ingreso de argumentos de entrada (Dimensión matriz, número de hilos)
 *      - Validación argumentos de entrada
 *      - Inicializar las matrices
 *      - Imprimir las matrices
 *      - Función para inicializar las matrices
 *      - Función para imprimir las matrices
 *      - Algoritmo clásico de multiplicación matrices
 *  - Se verifica el resultado
 *      - Función para multiplicación las matrices
 *      - Declaración vector de hilos
 *      - Creación de hilos según tamaño de vector de hilos
 *      - Crear estructura de datos que encapsule los argumentos de entrada de la función MM
 * Implementación de paralelismo: resolución de la multiplicación de matrices
 *  - Se crea el vector de hilos
 *  - Se tiene pendiente la exclusión de los hilos
 *  - Se pasa a globales las matrices
 *  - Encapsular los datos para enviarlos a la función MxM
 *  - Se desencapsulan los datos dentro de la función MxM (descomprimen)
*************************************************************************************/

#include "Interfaz.h"
#include <time.h> // Necesario para la función rand()


// DEFINICIÓN DE VARIABLES
double *mA = NULL, *mB = NULL, *mC = NULL;
struct timeval ini, fin;
int N = 0;

// IMPLEMENTACIÓN DE FUNCIONES AUXILIARES
void InicioMuestra() {
    gettimeofday(&ini, NULL);
}

/** Funcion para tomar tiempo final y presenta el resultado en ms*/
void FinMuestra() {
    gettimeofday(&fin, NULL);
    
    //Se resta el tiempo final menos el tiempo inicial
    double tiempo = (double)(fin.tv_sec - ini.tv_sec) * 1000.0 + 
                    (double)(fin.tv_usec - ini.tv_usec) / 1000.0;
    
    printf("\nTiempo total de ejecución: %.9lf ms\n", tiempo);
}

/** Funcion para inicializar la matriz con numeros aleatorios */
void iniMatriz(int n, double *m1, double *m2) {
    // Usa RAND para inicializar con valores aleatorios 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int idx = i * n + j;
            m1[idx] = (double)rand() / (RAND_MAX / 10.0);
            m2[idx] = (double)rand() / (RAND_MAX / 10.0);
        }
    }
}

/** Funcion para imprimir la matriz */
void imprMatrices(int n, double *matriz) {
    // CORRECCIÓN: Límite de impresión ajustado
    if (n <= 9) { 
        printf("\n######################################\n");
        printf("Matriz de %dx%d\n", n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%10.6f ", matriz[i * n + j]); 
            }
            printf("\n");
        }
        printf("######################################\n");
    } else {
        printf("\n######################################\n");
        printf("Matriz demasiado grande para imprimir completa (%dx%d)\n", n, n);
        printf("######################################\n");
    }
}

/** Algoritmo Clásico de Multiplicación */
void multiMatrizClasica(double *m1, double *m2, int n) {
    // Se mantiene la lógica de punteros que el compilador aceptó
    for (int i = 0; i < n; i++) {         
        for (int j = 0; j < n; j++) { 
            double sumaTemp = 0.0;
            for (int k = 0; k < n; k++) { 
                sumaTemp += m1[i * n + k] * m2[k * n + j];
            }
            mC[i * n + j] = sumaTemp; 
        }
    }
}
