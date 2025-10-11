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
 *  - Se desencapsulan los datos dentro de la función MxM (descomprimen)
*************************************************************************************/
#include "Interfaz.h" 
#include <time.h> // Necesario para la inicialización aleatoria

int main(int argc, char *argv[]) {
    //VALIDACIÓN Y LECTURA DE ARGUMENTOS
    if (argc != 2) { 
        fprintf(stderr, "Numero de argumentos incorrectos\n");
        fprintf(stderr, "Uso: %s <DIMENSION_N>\n", argv[0]);
        return -1;
    }

    // Inicializamos la semilla aleatoria para que las matrices cambien en cada ejecución
    srand(time(NULL)); 
    
    // Leemos N del argumento (argv[1])
    N = (int)strtod(argv[1], NULL); 
    
    if (N <= 0) {
        fprintf(stderr, "\nEl valor de la dimension (N) debe ser mayor que cero\n");
        return -1;
    }
    
    //RESERVA DE MEMORIA (usa N)
    size_t size = N * N * sizeof(double);
    mA = (double *)malloc(size);
    mB = (double *)malloc(size);
    // calloc inicializa mC con ceros
    mC = (double *)calloc(N * N, sizeof(double)); 

    if (!mA || !mB || !mC) {
        fprintf(stderr, "Error al asignar memoria.\n");
        if (mA) free(mA);
        if (mB) free(mB);
        if (mC) free(mC);
        return -1;
    }

    // INICIALIZACIÓN, CÁLCULO Y MEDICIÓN
    
    iniMatriz(N, mA, mB); //Llena las matrices A y B con números
    imprMatrices(N, mA);  // Muestra la matriz A
    imprMatrices(N, mB);  // Muestra la matriz B

    InicioMuestra(); // Empieza a contar el tiempo de ejecución
    multiMatrizClasica(mA, mB, N);  // Ejecuta la multiplicación
    FinMuestra();    // Detiene el contador de tiempo y muestra el resultado

    // IMPRESIÓN DEL RESULTADO 
    printf("\nResultado de la Multiplicación:\n");
    imprMatrices(N, mC); // Muestra el resultado de A x B 
    printf("\nFin del programa..............\n");
    
    // LIBERA LA MEMORIA
    free(mA);
    free(mB);
    free(mC);

    return 0;
}
