/*******************************************************
*            Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: Introducción a OpenMP
* Descripción:
*   Ejemplo del uso de la directiva parallel for con 
*   las cláusulas private, shared y lastprivate.
*******************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;     // Variable para el ciclo
    const int N = 1000;  // Número total de iteraciones
    int a = 50;          // Variable compartida
    int b = 0;           // Variable que guardará el último valor

    // Bucle paralelo: cada hilo ejecuta parte del ciclo
    // i es privada, a es compartida, b conserva el último valor calculado
    #pragma omp parallel for private(i) shared(a) lastprivate(b)
    for (i = 0; i < N; i++) {
        b = a + i;  // Cada hilo calcula su propio valor de b
    }

    // Se imprime el resultado final (esperado: a=50, b=1049)
    printf("a = %d b = %d (Se espera a=50 b=1049)\n", a, b);

    return 0;  // Fin del programa
}

