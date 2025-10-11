/**********************************************************
* Pontificia Universidad Javeriana			  *
* Autor: Diego Alejandro Mendoza Cruz			  *
* Fecha: 09/10/2025				          *
* Materia: Sistemas Operativos				  *
* Tema: Creación de procesos fork			  *
* Objetivo: Identificación del proceso padre e hijo       *
* *********************************************************/

#include <stdio.h>    // Para printf()
#include <unistd.h>   // Para fork(), getpid(), getppid()
#include <sys/types.h> // Para el tipo pid_t

int main() {
    // Variable para almacenar el valor retornado por fork()
    pid_t procesoID = fork();

    // Se comprueba si fork() falló al crear el proceso hijo.
    if (procesoID == -1) {
        perror("Error al crear el proceso hijo con fork()"); //Si retorna -1, imprime un mensaje de error
        return 1; // Retorna código de error
    }

    // 2. Identificación y ejecución del proceso hijo
    if (procesoID == 0) {
        // En el proceso hijo
        printf("Soy el proceso hijo.\n");
        printf("Mi nombre es Diego Mendoza!!!\n");
        printf("Mi ID es: %d \n", getpid());                //getpid() retorna el ID del proceso actual (el hijo).
        printf("El ID de mi Padre es: %d. \n\n", getppid());//getppid() retorna el ID del proceso padre que lo creó.
    }
    // 3. Identificación y ejecución del proceso padre
    else {
        // En el proceso padre
        printf("Hola, soy el proceso padre\n");
        printf("Mi ID es: %d \n", getpid());                // 'getpid()' retorna el ID del proceso actual (el padre).
        printf("El ID de mi hijo es: %d\n\n", procesoID);   // 'procesoID' contiene el ID del hijo que acaba de crear.
    
    }

    // 4. Código que se ejecuta por ambos procesos ya que se encuentra fuera del if/else
    printf("Fin del programa!!\n");
    
    //Finalización exitosa del programa.
    return 0;
}
