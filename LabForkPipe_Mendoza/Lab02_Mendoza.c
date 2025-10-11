/**************************************************************
* Pontificia Universidad Javeriana			      *
* Autor: Diego Alejandro Mendoza Cruz 			      *
* Fecha: 09/10/2025					      *
* Materia: Sistemas Operativos				      *
* Tema: Creación de procesos fork			      *
* Objetivos: - Identificación del proceso padre e hijo        *
*            - Procesos Duplicados	                      *                        *
***************************************************************/
#include <stdio.h>    // Para printf()
#include <stdlib.h>   // Para el uso general 
#include <unistd.h>   // Para fork(), getpid(), getppid()
#include <sys/types.h> // Para el tipo pid_t

int main() {
    
    printf("\n\nPrograma Principal!!!\n\n");
    
    // Se invoca la creación del proceso en una variable para identificarlo
    pid_t procesoID = fork();
    
    // Se duplica el proceso
    
    // Manejo de Error
    if (procesoID == -1) {
        printf("Error de llamada a fork()!\n");
        return 1; // Código de error
    }
    
    // Código Ejecutado por el PROCESO HIJO
    if (procesoID == 0) {
        // En el proceso hijo (fork() retorna 0)
        printf("\nSoy Luke Skywalker!!! ID = %d (Mi Padre es: %d)\n", getpid(), getppid());
        
        // El hijo cuenta del 0 al 5
        for (int i = 0; i <= 5; i++) {
            printf("Valor HIJO: == %d ==\n", i);
        }
    } 
    
    
    // Código Ejecutado por el PROCESO PADRE
    else { // if (procesoID > 0) // El padre recibe el PID del hijo (que es > 0)
        // En el proceso padre
        printf("\nSoy tu PADRE!!! ID = %d (Mi Hijo es: %d)\n", getpid(), procesoID);
        
        // El padre cuenta del 5 al 0
        for (int j = 5; j >= 0; j--) {
            printf("Valor PADRE: == %d ==\n", j);
        }
    }
    
    // Esta línea puede imprimirse en cualquier orden dependiendo del planificador del SO.
    printf("\nFin de ejecución del Proceso.\n");
    
    return 0;
}
