/*******************************************************************
 * Nombres: Diego Mendoza, Juan Diego Ariza
 * Materia: Sistemas Operativos
 * Profesor: John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha: Noviembre/2025
 * Tema: Taller de Evaluación de Rendimiento
 * Descripción: Multiplicación de matrices con algoritmo clásico
 *              usando paralelización OpenMP
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

// Variables globales para medición de tiempo
struct timeval inicio, fin; 

/**
 * Función: InicioMuestra
 * Captura el tiempo inicial antes de ejecutar el algoritmo.
 * Usa gettimeofday() para obtener microsegundos de precisión.
 */
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

/**
 * Función: FinMuestra
 * Calcula el tiempo transcurrido desde InicioMuestra().
 * Resta los valores de inicio a fin para obtener el delta.
 * Convierte el resultado a microsegundos e imprime.
 */
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;  // Restar microsegundos
	fin.tv_sec  -= inicio.tv_sec;   // Restar segundos
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo);
}

/**
 * Función: impMatrix
 * Imprime una matriz en formato legible.
 * Solo imprime si la dimensión es menor a 9 (matrices pequeñas).
 * Inserta saltos de línea cada D elementos para visualizar filas.
 */
void impMatrix(double *matrix, int D){
	if(D < 9){
		printf("\n");
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");  // Nueva fila cada D elementos
			printf("%.2f ", matrix[i]);
		}
		printf("\n**-----------------------------**\n");
	}
}

/**
 * Función: iniMatrix
 * Inicializa dos matrices con valores aleatorios.
 * m1 recibe valores entre 1.0 y 5.0
 * m2 recibe valores entre 2.0 y 9.0
 * Usa rand() normalizado por RAND_MAX para generar doubles.
 */
void iniMatrix(double *m1, double *m2, int D){
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = (double)rand()/RAND_MAX*(5.0-1.0);  // Rango [1.0, 5.0]	
		*m2 = (double)rand()/RAND_MAX*(9.0-2.0);  // Rango [2.0, 9.0]	
	}
}

/**
 * Función: multiMatrix
 * Implementa la multiplicación de matrices usando el algoritmo clásico.
 */
void multiMatrix(double *mA, double *mB, double *mC, int D){
	double Suma, *pA, *pB;
	
	// Crear región paralela donde múltiples hilos ejecutarán el código
	#pragma omp parallel
	{
	// Distribuir el bucle for entre los hilos disponibles
	#pragma omp for
	for(int i=0; i<D; i++){  // Iterar sobre filas de A
		for(int j=0; j<D; j++){  // Iterar sobre columnas de B
			pA = mA+i*D;  // Posicionar pA al inicio de la fila i de A	
			pB = mB+j;    // Posicionar pB al inicio de la columna j de B
			Suma = 0.0;
			
			// Producto punto entre fila i de A y columna j de B
			for(int k=0; k<D; k++, pA++, pB+=D){
				Suma += *pA * *pB;  // A[i][k] * B[k][j]
			}
			mC[i*D+j] = Suma;  // Almacenar resultado en C[i][j]
		}
	}
	}  // Fin de región paralela
}

/**
 * Función: main
 * Función principal del programa.
 */
int main(int argc, char *argv[]){
	// Validar número de argumentos
	if(argc < 3){
		printf("\n Use: $./mmClasicaOpenMP SIZE Hilos \n\n");
		exit(0);
	}

	// Leer argumentos de línea de comandos
	int N = atoi(argv[1]);   // Tamaño de la matriz (NxN)
	int TH = atoi(argv[2]);  // Número de hilos a usar
	
	// Reservar memoria dinámica para las tres matrices
	// calloc inicializa todos los valores en 0
	double *matrixA  = (double *)calloc(N*N, sizeof(double));
	double *matrixB  = (double *)calloc(N*N, sizeof(double));
	double *matrixC  = (double *)calloc(N*N, sizeof(double));

	// Inicializar semilla del generador de números aleatorios
	srand(time(NULL));
	
	// Configurar el número de hilos que usará OpenMP
	omp_set_num_threads(TH);

	// Llenar matrices A y B con valores aleatorios
	iniMatrix(matrixA, matrixB, N);
	
	// Imprimir matrices de entrada (solo para verificación con matrices pequeñas)
	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

	// Capturar tiempo inicial
	InicioMuestra();
	
	// Realizar la multiplicación de matrices (operación a medir)
	multiMatrix(matrixA, matrixB, matrixC, N);
	
	// Capturar tiempo final y calcular diferencia
	FinMuestra();

	// Imprimir matriz resultado
	impMatrix(matrixC, N);

	// Liberar memoria reservada
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
	return 0;
}
