/*******************************************************************
 * Nombre: Diego Mendoza, Juan Diego Ariza
 * Materia: Sistemas Operativos
 * Profesor: John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha: Noviembre 11 de 2025
 * Tema: Taller de Evaluación de Rendimiento
 * Descripción: Multiplicación de matrices usando algoritmo de
 *              matriz transpuesta (Filas x Filas) con OpenMP.
 *              Mejora la localidad espacial en memoria caché.
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

// Variables para medición de tiempo
struct timeval inicio, fin; 

/**
 * Función: InicioMuestra
 * Captura el tiempo inicial de ejecución.
 */
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

/**
 * Función: FinMuestra
 * Calcula y muestra el tiempo transcurrido en microsegundos.
 */
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo);
}

/**
 * Función: impMatrix
 * Imprime una matriz en dos formatos diferentes.
 */
void impMatrix(double *matrix, int D, int t){
	int aux = 0;
	if(D < 6)
		switch(t){
			case 0:  // Impresión normal fila por fila
				for(int i=0; i<D*D; i++){
					if(i%D==0) printf("\n");
					printf("%.2f ", matrix[i]);
				}
				printf("\n  - \n");
				break;
			case 1:  // Impresión transpuesta columna por columna
				while(aux<D){
					for(int i=aux; i<D*D; i+=D)
						printf("%.2f ", matrix[i]);
					aux++;
					printf("\n");
				}	
				printf("\n  - \n");
				break;
			default:
				printf("Sin tipo de impresión\n");
		}
}

/**
 * Función: iniMatrix
 * Inicializa dos matrices con valores aleatorios.
 * m1: Valores entre 1.0 y 5.0
 * m2: Valores entre 5.0 y 9.0
 */
void iniMatrix(double *m1, double *m2, int D){
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = (double)rand()/RAND_MAX*(5.0-1.0);	
		*m2 = (double)rand()/RAND_MAX*(9.0-5.0);		
	}
}

/*
 * Función: multiMatrixTrans
 * Multiplica matrices usando el algoritmo de matriz transpuesta.
 */
void multiMatrixTrans(double *mA, double *mB, double *mC, int D){
	double Suma, *pA, *pB;
	
	// Crear región paralela
	#pragma omp parallel
	{
	// Distribuir el bucle entre hilos
	#pragma omp for
	for(int i=0; i<D; i++){  // Iterar sobre filas de A
		for(int j=0; j<D; j++){  // Iterar sobre filas de B' (columnas de B)
			pA = mA+i*D;  // Apuntar al inicio de la fila i de A
			pB = mB+j*D;  // Apuntar al inicio de la fila j de B' 
			Suma = 0.0;
			
			// Producto punto de fila i de A con fila j de B'
			for(int k=0; k<D; k++, pA++, pB++){
				Suma += *pA * *pB;  // A[i][k] * B'[j][k] = A[i][k] * B[k][j]
			}
			mC[i*D+j] = Suma;  // Guardar resultado en C[i][j]
		}
	}
	}  // Fin de región paralela
}

/**
 * Función: main
 * Función principal del programa.
 */
int main(int argc, char *argv[]){
	// Validar argumentos
	if(argc < 3){
		printf("\n Use: $./mmFilasOpenMP SIZE Hilos \n\n");
		exit(0);
	}

	// Leer argumentos
	int N = atoi(argv[1]);   // Tamaño de matriz
	int TH = atoi(argv[2]);  // Número de hilos
	
	// Reservar memoria para las tres matrices
	double *matrixA  = (double *)calloc(N*N, sizeof(double));
	double *matrixB  = (double *)calloc(N*N, sizeof(double));
	double *matrixC  = (double *)calloc(N*N, sizeof(double));
	
	// Inicializar generador de números aleatorios
	srand(time(NULL));

	// Configurar número de hilos OpenMP
	omp_set_num_threads(TH);

	// Inicializar matrices A y B
	iniMatrix(matrixA, matrixB, N);

	// Imprimir matrices de entrada
	// matrixA en formato normal (fila por fila)
	impMatrix(matrixA, N, 0);
	// matrixB en formato transpuesto para visualización
	impMatrix(matrixB, N, 1);

	// Iniciar medición de tiempo
	InicioMuestra();
	
	// Realizar multiplicación con algoritmo transpuesto
	multiMatrixTrans(matrixA, matrixB, matrixC, N);
	
	// Finalizar medición de tiempo
	FinMuestra();

	// Imprimir matriz resultado
	impMatrix(matrixC, N, 0);

	// Liberar memoria
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
	return 0;
}
