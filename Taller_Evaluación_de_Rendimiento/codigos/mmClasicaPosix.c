/*******************************************************************
 * Nombre: Diego Mendoza, Juan Diego Ariza
 * Materia: Sistemas Operativos
 * Profesor: John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha: Noviembre 11 de 2025
 * Tema: Taller de Evaluación de Rendimiento
 * Descripción: Multiplicación de matrices con algoritmo clásico
 *              usando paralelización con hilos POSIX (Pthreads)
 *******************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// Mutex para sincronización de hilos
pthread_mutex_t MM_mutex;

// Matrices globales compartidas por todos los hilos
double *matrixA, *matrixB, *matrixC;

struct parametros{
	int nH;    // Numero total de hilos 
	int idH;   // Identificador ùnico del hilo
	int N;     // Dimension de las matrices cuadradas
};

// Variables para medición de tiempo
struct timeval inicio, fin;

/**
 * Función: InicioMuestra
 * Captura el tiempo inicial usando gettimeofday().
 */
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

/**
 * Función: FinMuestra
 * Calcula el tiempo transcurrido en microsegundos.
 * Resta el tiempo inicial del final e imprime el resultado.
 */
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

/**
 * Función: iniMatrix
 * Inicializa dos matrices con valores aleatorios.
 */
void iniMatrix(double *m1, double *m2, int D){ 
	for(int i = 0; i < D*D; i++, m1++, m2++){
		*m1 = (double)rand()/RAND_MAX*(5.0-1.0); 
		*m2 = (double)rand()/RAND_MAX*(9.0-5.0); 
	}	
}

/**
 * Función: impMatrix
 * Imprime una matriz en formato legible.
 * Solo imprime matrices menores a 9x9.
 */
void impMatrix(double *matriz, int D){
	if(D < 9){
		for(int i = 0; i < D*D; i++){
			if(i%D==0) printf("\n");  // Nueva fila cada D elementos
			printf(" %.2f ", matriz[i]);
		}	
		printf("\n>-------------------->\n");
	}
}

/**
 * Función: multiMatrix
 * Función ejecutada por cada hilo para multiplicar matrices.
 */
void *multiMatrix(void *variables){
	// Convertir el puntero void* a la estructura parametros
	struct parametros *data = (struct parametros *)variables;
	
	int idH		= data->idH;  // ID de este hilo
	int nH		= data->nH;   // Número total de hilos
	int D		= data->N;    // Dimensión de la matriz
	
	// Calcular rango de filas para este hilo
	int filaI	= (D/nH)*idH;        // Fila inicial (inclusiva)
	int filaF	= (D/nH)*(idH+1);    // Fila final (exclusiva)
	
	double Suma, *pA, *pB;

	// Iterar sobre el rango de filas asignado
	for (int i = filaI; i < filaF; i++){
		for (int j = 0; j < D; j++){
			pA = matrixA + i*D;  // Apuntar al inicio de la fila i de A
			pB = matrixB + j;    // Apuntar a la columna j de B
			Suma = 0.0;
			
			// Producto punto de fila i de A con columna j de B
			for (int k = 0; k < D; k++, pA++, pB+=D){
				Suma += *pA * *pB;  // A[i][k] * B[k][j]
			}
			matrixC[i*D+j] = Suma;  // Guardar resultado en C[i][j]
		}
	}

	// Sincronización 
	pthread_mutex_lock (&MM_mutex);
	pthread_mutex_unlock (&MM_mutex);
	
	// Terminar el hilo
	pthread_exit(NULL);
}

/**
 * Función: main
 * Función principal del programa.
 */
int main(int argc, char *argv[]){
	// Validar argumentos
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);	
	}
	
	// Leer argumentos
	int N = atoi(argv[1]);          // Tamaño de matriz
	int n_threads = atoi(argv[2]);  // Número de hilos

	// Declarar arreglo de identificadores de hilos
	pthread_t p[n_threads];
	pthread_attr_t atrMM;  // Atributos de los hilos

	// Reservar memoria para las tres matrices
	matrixA  = (double *)calloc(N*N, sizeof(double));
	matrixB  = (double *)calloc(N*N, sizeof(double));
	matrixC  = (double *)calloc(N*N, sizeof(double));

	// Inicializar matrices A y B con valores aleatorios
	iniMatrix(matrixA, matrixB, N);
	
	// Imprimir matrices de entrada (solo para verificación)
	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

	// Iniciar medición de tiempo
	InicioMuestra();

	// Inicializar mutex para sincronización
	pthread_mutex_init(&MM_mutex, NULL);
	
	// Inicializar atributos de hilos
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

	// Crear los hilos
	for (int j=0; j<n_threads; j++){
		// Reservar memoria para los parámetros de este hilo
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;          // Asignar ID del hilo
		datos->nH  = n_threads;  // Pasar número total de hilos
		datos->N   = N;          // Pasar tamaño de matriz

		// Crear el hilo j que ejecutará multiMatrix
		pthread_create(&p[j],&atrMM,multiMatrix,(void *)datos);
	}

	// Esperar a que todos los hilos terminen su ejecución
	for (int j=0; j<n_threads; j++)
		pthread_join(p[j],NULL);

	// Finalizar medición de tiempo
	FinMuestra();
	
	// Imprimir matriz resultado
	impMatrix(matrixC, N);

	// Liberar memoria de las matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);

	// Destruir atributos y mutex
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);

	return 0;
}
