/**********************************************************
* Pontificia Universidad Javeriana			  
* Autor: Diego Mendoza					  
* Fecha: 09/10/2025					  
* Tema: Creación de procesos fork			  
*   - Identificación del proceso padre e hijo	  	  
*   - Procesos duplicados				  
*   - Comunicacion entre procesos con PIPE                
* ********************************************************/

#include <stdio.h>     // Para printf() y perror()
#include <stdlib.h>    // Para exit(), EXIT_FAILURE
#include <unistd.h>    // Para fork(), pipe(), read(), write(), close()
#include <string.h>    // Se incluye solo si se usa funciones como strlen o algo parecido. 
#include <sys/types.h> // Para el tipo pid_t

int main() {
    
    // *Creación de tubería de comunicación*
    int pipefd[2]; 
    
    // *Id del hijo como proceso*
    pid_t hijo_PID;
    
    // *Verificación si el PIPE*
    if (pipe(pipefd) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE); 
    }
    
    // *Se invoca la creación del proceso en un INT para identificar*
    hijo_PID = fork();
    
    // *Verificación creación del proceso*
    if (hijo_PID == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    // *Se duplica el proceso*
    // CÓDIGO DEL PROCESO HIJO (Lector)
    if (hijo_PID == 0) { 
        printf("\nSoy Luck Skywalker!!! ID = %d \n", getpid());
        
        // *Cerrar SALIDA para lectura*
        close(pipefd[1]); 
        
        // *Capacidad de vector caracteres*
        char mensaje[100]; // Buffer de 100 caracteres
        
        // *lectura de bytes*
        // Se llama a read. El resultado no se almacena en una variable.
        if (read(pipefd[0], mensaje, sizeof(mensaje)) == -1) {
            // *Verificación de lectura del mensaje*
            perror("LECTURA!!");
            exit(EXIT_FAILURE);
        }
        
        
        // Asumimos que el buffer fue llenado correctamente.
        printf("HIJO recibe:  %s  \n", mensaje); 
        
        // *Cerrar ENTRADA*
        close(pipefd[0]);
    } 
    
    
    // CÓDIGO DEL PROCESO PADRE (Escritor)
   
    else { 
        
        // *Envio del mensaje desde el PADRE*
        printf("\nSoy DARK trae el pan y las gaseosas...! \n");
        
        // *Cerrar ENTRADA para lectura*
        close(pipefd[0]);
        
        // Mensaje a enviar
        char *mensaje = "Hola Luke, aqui tu padre! Toma tu pan y tu gaseosa.";
        
        // *escritura de bytes*
        // Se llama a write. El resultado no se almacena en una variable.
        if (write(pipefd[1], mensaje, 50) == -1) { 
            // *Verificación de escritura del mensaje*
            perror("ESCRITURA!");
            exit(EXIT_FAILURE);
        }
        
        // *Cerrar SALIDA*
        close(pipefd[1]);
    }
    printf("Fin del programa.");
    return 0;
}
