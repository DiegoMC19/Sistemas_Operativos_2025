/************************************************************
*                  Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: OpenMP - Cálculo de sumatoria con función seno (Lab06)
* Descripción:
*   Programa que calcula la sumatoria de valores seno usando
*   paralelización con OpenMP y la cláusula reduction.
*   El usuario puede ingresar el número de hilos y la cantidad
*   de repeticiones (n). Se mide el tiempo total de ejecución
*   del cálculo paralelo y se imprime junto con la sumatoria final.
************************************************************/

#include <stdio.h>   // Entrada y salida estándar
#include <stdlib.h>  // Conversión de cadenas y uso de atoi
#include <omp.h>     // Librería de paralelismo OpenMP
#include <math.h>    // Función sin()

// Función f: calcula la suma de senos desde "start" hasta "finish"
double f(int i) {
    double j, start = i * (i + 1) / 2, finish = start + i;
    int j_int;
    double return_val = 0;

    for (j_int = start; j_int < finish; j_int++)
        return_val += sin(j_int);

    return return_val;
}

// Función principal
int main(int argc, char *argv[]) {
    double sum = 0;
    int i, n, num_hilos;
    double inicio, fin, tiempo_total;

    // Validar argumentos
    if (argc != 3) {
        printf("Uso: %s <num_hilos> <n>\n", argv[0]);
        printf("Ejemplo: %s 4 30000\n", argv[0]);
        return 1;
    }

    // Asignar valores desde la línea de comandos
    num_hilos = atoi(argv[1]);
    n = atoi(argv[2]);

    // Establecer número de hilos
    omp_set_num_threads(num_hilos);

    // Iniciar medición de tiempo
    inicio = omp_get_wtime();

    // Región paralela
    #pragma omp parallel
    {
        // Bucle paralelo con reducción
        #pragma omp for reduction(+: sum)
        for (i = 0; i < n; i++) {
            sum += f(i);
        }
    }

    // Finalizar medición de tiempo
    fin = omp_get_wtime();
    tiempo_total = fin - inicio;

    // Mostrar resultados
    printf("\n------------------------------------------\n");
    printf("Número de hilos usados: %d\n", num_hilos);
    printf("Número de iteraciones: %d\n", n);
    printf("Sumatoria total: %.2f\n", sum);
    printf("Tiempo total de ejecución: %.6f segundos\n", tiempo_total);
    printf("------------------------------------------\n");

    return 0;
}

