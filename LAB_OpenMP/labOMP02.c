/************************************************************************************************************************
*			Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: Introducción a OPENMP
* Descripción:
*	- Uso de la directiva y función para hacer uso de OpenMP
*	- Uso de omp_set_num_threads() para configurar hilos específicos
*	- Validación de entrada y ejecución de región paralela
*************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){
	// Validación: Requiere número de hilos como argumento
	if(argc !=2){
	    printf("Error:  \n\t$./ejecutable numHilos\n");
	    exit(0);
	}
	// Convertir argumento a entero
	int numHilos = (int) atoi(argv[1]);
	
	// Obtener y mostrar hilos máximos del sistema
	int maxCores = omp_get_max_threads();
	printf("Numero maximo de cores del SO: %d  \n", maxCores);

	/*Se fija el numero de hilos deseado: numHilos*/
	omp_set_num_threads(numHilos);
	printf("Numero hilos fijados: %d \n",numHilos);

	/*Region Paralela por OMP: Cada hilo imprime su ID*/
#pragma omp parallel
	{ 
	  printf("Hilo ID en Region Paralela: %d \n",omp_get_thread_num());
	}

}
