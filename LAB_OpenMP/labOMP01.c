/*****************************************************************
*			Pontificia Universidad Javeriana
*
* Autor: Diego Mendoza
* Profesor: J Corredor
* Materia: Sistemas Operativos
* Tema: Introducción a OPENMP
* Descripción:
*	- Uso de la directiva y función para hacer uso de OpenMP
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char *argv[]){
	int maxCores = omp_get_max_threads(); // Obtiene el numero máximo de hilos disponibles en el sistema
	printf("Numero maximo de cores del SO: %d  \n", maxCores); // Imprime el número máximo de cores
	/*Region Paralela por OMP*/
#pragma omp parallel
	{ 
	  printf("Hilo ID en Region Paralela: %d \n",omp_get_thread_num()); // Cada hilo imprime su ID unico
	}

}
