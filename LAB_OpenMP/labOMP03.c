/**************************************************************************
*                    Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: Introducción a OPENMP
* Descripción:
*       - Paralelización de bucles for con OpenMP
*       - Uso de cláusula private para variables privadas por hilo
*       - Demostración de suma paralela con variables compartidas
**************************************************************************/

#include <omp.h>      
#include <stdio.h>
#include <stdlib.h>

int main(){
    int i; // Variable para el ciclo
    const int N = 1000;   // Número total de iteraciones
    int a = 50; // Variable para la suma
    int b = 0;  // Variable para acumular

    // Paralelización del bucle for
    // private(i): cada hilo tiene su propia copia de 'i'
    // private(a): cada hilo tiene su propia copia de 'a' (no afecta la original)
    #pragma omp parallel for private(i) private(a)
    for(i=0; i<N; i++){
        b = a + i;  // 'b' es compartida entre hilos (condición de carrera)
    }
    
    printf("a = %d b = %d (Se espera a=50 b = 1049)\n", a, b);
    return 0;
}
