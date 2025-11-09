/*******************************************************************
 * Nombres: Diego Mendoza, Juan Diego Ariza 
 * Materia: Sistemas Operativos
 * Profesor: John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha: Noviembre 11 de 2025
 * Tema: Taller de Evaluación de Rendimiento
 * Descripción: Multiplicación de matrices con algoritmo clásico
 *              usando paralelización con procesos Fork.
 *              Nota: Este programa tiene limitaciones porque los
 *              procesos hijos no comparten memoria con el padre.
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

// Variables para medición de tiempo
struct timeval inicio, fin;

/**
 * Función: InicioMuestra
 * Captura el tiempo inicial antes de la operación.
 */
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

/**
 * Función: FinMuestra
 * Calcula el tiempo transcurrido en microsegundos e imprime.
 */
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

/**
 * Función: multiMatrix
 * Multiplica un rango específico de filas de la matriz A por B.
 */
void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
	double Suma, *pA, *pB;
	
	// Iterar sobre el rango de filas asignado
	for (int i = filaI; i < filaF; i++) {
		for (int j = 0; j < D; j++) {
			Suma = 0.0;
			pA = mA+i*D;  // Apuntar al inicio de la fila i de A
			pB = mB+j;    // Apuntar a la columna j de B
			
			// Producto punto de fila i de A con columna j de B
			for (int k = 0; k < D; k++, pA++, pB+=D) {
				Suma += *pA * *pB;
			}
			mC[i*D+j] = Suma; 
		}
	}
}

/**
 * Función: impMatrix
 * Imprime una matriz en formato legible.
 * Solo imprime matrices menores a 9x9.
 */
void impMatrix(double *matrix, int D) {
	if (D < 9) {
		printf("\nImpresión	...\n");
		for (int i = 0; i < D*D; i++, matrix++) {
			if(i%D==0) printf("\n");  // Nueva fila
			printf(" %.2f ", *matrix);
		}
		printf("\n ");
	}
}

/**
 * Función: iniMatrix
 * Inicializa dos matrices con valores aleatorios.
 * mA: Valores entre 1.0 y 5.0
 * mB: Valores entre 5.0 y 9.0
 */
void iniMatrix(double *mA, double *mB, int D){
	for (int i = 0; i < D*D; i++, mA++, mB++){
		*mA = (double)rand()/RAND_MAX*(5.0-1.0); 
		*mB = (double)rand()/RAND_MAX*(9.0-5.0); 
	}
}

/**
 * Función: main
 * Función principal del programa.
 */
int main(int argc, char *argv[]) {
	// Validar argumentos
	if (argc < 3) {
		printf("\n \t\tUse: $./mmClasicaFork Size Procesos \n");
		exit(0);
	}
	
	// Leer argumentos
	int N      = (int) atoi(argv[1]);  // Tamaño de matriz
	int num_P  = (int) atoi(argv[2]);  // Número de procesos
	
	// Reservar memoria para las matrices
	double *matA = (double *) calloc(N*N, sizeof(double));
	double *matB = (double *) calloc(N*N, sizeof(double));
	double *matC = (double *) calloc(N*N, sizeof(double));

	// Inicializar semilla de números aleatorios
	srand(time(0)); 
	
	// Inicializar matrices A y B
	iniMatrix(matA, matB, N);
	
	// Imprimir matrices de entrada
	impMatrix(matA, N);
	impMatrix(matB, N);
	
	// Calcular cuántas filas procesará cada proceso
	int rows_per_process = N/num_P;

	// Iniciar medición de tiempo
	InicioMuestra();

	// Crear procesos hijos
	for (int i = 0; i < num_P; i++) {
		pid_t pid = fork();  // Crear un proceso hijo
		
		if (pid == 0) {  // Código ejecutado por el HIJO
			// Calcular el rango de filas para este proceso
			int start_row = i * rows_per_process;
			int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;
			
			// El hijo calcula su rango de filas
			multiMatrix(matA, matB, matC, N, start_row, end_row); 
			
			// Imprimir resultados del hijo (solo para matrices pequeñas)
			if(N<9){
				printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row-1);
				for (int r = start_row; r < end_row; r++) {
					for (int c = 0; c < N; c++) {
						printf(" %.2f ", matC[N*r+c]);
					}
					printf("\n");
				}
			}
			
			// El hijo termina aquí
			exit(0); 
			
		} else if (pid < 0) {  // Error al crear proceso
			perror("fork failed");
			exit(1);
		}
		// Si pid > 0, es el PADRE y continúa el bucle para crear más hijos
	}
	
	// El proceso padre espera a que todos los hijos terminen
	for (int i = 0; i < num_P; i++) {
		wait(NULL);  // Esperar a cualquier hijo
	}
	
	// Finalizar medición de tiempo
	FinMuestra(); 

	// Liberar memoria
	free(matA);
	free(matB);
	free(matC);

	return 0;
}
