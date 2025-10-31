/*******************************************************************
 * Nombre: Juan Diego Ariza, Diego Alejandro Mendoza               *
 * Materia: Sistemas Operativos                                    *
 * Profesor: John Corredor, Ph.D.                                  *
 * Universidad: Pontificia Universidad Javeriana                   *
 * Fecha: 29/10/2025                                               *
 * Tema: Taller 02 - Procesos y Comunicación entre Procesos        *
 * Descripción: Programa que implementa 4 procesos usando fork()   *
 *              para calcular sumas de arreglos. Usa pipes para    *
 *              comunicación entre procesos (Grand hijo, Segundo   *
 *              hijo, Primer hijo y Padre).                        *
 ******************************************************************/ 

/*
 * Librerías necesarias 
 */
#include <stdio.h>      // Para printf, fprintf, fopen, fscanf, etc.
#include <stdlib.h>     // Para malloc, free, atoi, exit
#include <unistd.h>     // Para fork, pipe, read, write, close
#include <sys/wait.h>   // Para wait()

/*
 * Esta función se encarga de abrir un archivo y leer los números enteros
 * que contiene. Los números deben estar separados por espacios.
 */
int* leer_archivo(const char* nombre_archivo, int n) {
    // Intentamos abrir el archivo en modo lectura
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir archivo");
        exit(1);
    }
    
    // Reservamos memoria para guardar n enteros
    int* arreglo = (int*)malloc(n * sizeof(int));
    if (arreglo == NULL) {
        perror("Error al asignar memoria");
        fclose(archivo);
        exit(1);
    }
    
    // Leemos cada número del archivo y lo guardamos en el arreglo
    for (int i = 0; i < n; i++) {
        if (fscanf(archivo, "%d", &arreglo[i]) != 1) {
            fprintf(stderr, "Error al leer elemento %d\n", i);
            free(arreglo);
            fclose(archivo);
            exit(1);
        }
    }
    
    fclose(archivo);
    return arreglo;
}

/*
 * Función que recorre todo el arreglo y va sumando cada elemento
 * Retorna la suma de todos los elementos
 */
int calcular_suma(int* arreglo, int n) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += arreglo[i];
    }
    return suma;
}

int main(int argc, char* argv[]) {
    /*
     *Verifica que el programa reciba los 4 argumentos necesarios:
     * N1, archivo00, N2, archivo01
     */
    if (argc != 5) {
        printf("Uso: %s N1 archivo00 N2 archivo01\n", argv[0]);
        return 1;
    }
    
    /*
     * Extrae los argumentos de la línea de comandos
     */
    int N1 = atoi(argv[1]);
    char* archivo00 = argv[2];
    int N2 = atoi(argv[3]);
    char* archivo01 = argv[4];
    
    printf("=== Taller de Procesos ===\n");
    printf("N1=%d, archivo00=%s\n", N1, archivo00);
    printf("N2=%d, archivo01=%s\n\n", N2, archivo01);
    
    /*
     * Se leen los dos archivos y se cargan los datos en memoria dinámica
     */
    int* arreglo1 = leer_archivo(archivo00, N1);
    int* arreglo2 = leer_archivo(archivo01, N2);
    
    /*
     * Se crean 3 pipes para la comunicación entre procesos
     * Cada pipe tiene dos extremos: [0] para leer y [1] para escribir
     * 
     * pipe_grandhijo: para que el Grand hijo envíe sumaA al Padre
     * pipe_hijo2: para que el Segundo hijo envíe sumaB al Padre  
     * pipe_hijo1: para que el Primer hijo envíe suma_total al Padre
     */
    int pipe_grandhijo[2];
    int pipe_hijo2[2];
    int pipe_hijo1[2];
    
    if (pipe(pipe_grandhijo) == -1 || pipe(pipe_hijo2) == -1 || pipe(pipe_hijo1) == -1) {
        perror("Error al crear pipes");
        free(arreglo1);
        free(arreglo2);
        return 1;
    }
    
    /*
     * Creamos el PRIMER HIJO usando fork()
     * fork() retorna:
     *   - 0 si estamos en el proceso hijo
     *   - PID del hijo si estamos en el padre
     *   - -1 si hay error
     */
    pid_t primer_hijo = fork();
    
    if (primer_hijo < 0) {
        perror("Error al crear primer hijo");
        free(arreglo1);
        free(arreglo2);
        return 1;
    }
    
    if (primer_hijo == 0) {
        /*
         * Este proceso calcula la suma total de ambos arreglos
         */
        close(pipe_hijo1[0]); // Cerramos lectura porque solo vamos a escribir
        
        /*
         * Desde el Primer hijo, creamos el SEGUNDO HIJO
         */
        pid_t segundo_hijo = fork();
        
        if (segundo_hijo < 0) {
            perror("Error al crear segundo hijo");
            exit(1);
        }
        
        if (segundo_hijo == 0) {
            /*
             * Este proceso calcula la sumaB del fichero01
             */
            close(pipe_hijo2[0]); // Solo escribimos en este pipe
            
            /*
             * Desde el Segundo hijo, creamos el GRAND HIJO
             */
            pid_t grand_hijo = fork();
            
            if (grand_hijo < 0) {
                perror("Error al crear grand hijo");
                exit(1);
            }
            
            if (grand_hijo == 0) {
                /*
                 *Este proceso calcula la sumaA del fichero00
                 */
                
                close(pipe_grandhijo[0]); // Solo escribimos aquí
                
                // Calculamos la suma del primer arreglo
                int sumaA = calcular_suma(arreglo1, N1);
                
                printf("[Grand hijo] sumaA = %d\n", sumaA);
                
                /*
                 * Enviamos el resultado al Padre a través del pipe
                 * write() escribe los datos en el extremo [1] del pipe
                 */
                write(pipe_grandhijo[1], &sumaA, sizeof(sumaA));
                close(pipe_grandhijo[1]);
                
                // Liberamos la memoria antes de terminar
                free(arreglo1);
                free(arreglo2);
                exit(0);
            }
            
            /*
             * El SEGUNDO HIJO continúa ejecutándose después de crear al Grand hijo
             */
            
            // Calculamos la suma del segundo arreglo
            int sumaB = calcular_suma(arreglo2, N2);
            
            printf("[Segundo hijo] sumaB = %d\n", sumaB);
            
            // Enviamos sumaB al Padre
            write(pipe_hijo2[1], &sumaB, sizeof(sumaB));
            close(pipe_hijo2[1]);
            
            /*
             * wait(NULL) hace que el Segundo hijo espere a que termine el Grand hijo
             * Esto asegura que el Grand hijo termine antes que su padre
             */
            wait(NULL);
            
            // Liberamos memoria y terminamos
            free(arreglo1);
            free(arreglo2);
            exit(0);
        }
        
        /*
         * El PRIMER HIJO continúa ejecutándose después de crear al Segundo hijo
         */
        
        // Calculamos la suma total sumando ambos arreglos
        int suma_total = calcular_suma(arreglo1, N1) + calcular_suma(arreglo2, N2);
        
        printf("[Primer hijo] Suma total = %d\n", suma_total);
        
        // Enviamos la suma total al Padre
        write(pipe_hijo1[1], &suma_total, sizeof(suma_total));
        close(pipe_hijo1[1]);
        
        // Esperamos a que termine el Segundo hijo (y por ende también el Grand hijo)
        wait(NULL);
        
        // Liberamos memoria y terminamos
        free(arreglo1);
        free(arreglo2);
        exit(0);
    }
    
    /*
     * Cerramos los extremos de escritura de los pipes porque
     * el Padre solo va a leer, no a escribir
     */
    close(pipe_grandhijo[1]);
    close(pipe_hijo2[1]);
    close(pipe_hijo1[1]);
    
    // Variables donde guardaremos los resultados recibidos
    int sumaA, sumaB, suma_total;
    
    /*
     * Leemos los resultados que enviaron los procesos hijos
     * read() bloquea hasta que haya datos disponibles en el pipe
     */
    read(pipe_grandhijo[0], &sumaA, sizeof(sumaA));
    read(pipe_hijo2[0], &sumaB, sizeof(sumaB));
    read(pipe_hijo1[0], &suma_total, sizeof(suma_total));
    
    // Ya leímos todo, cerramos los extremos de lectura
    close(pipe_grandhijo[0]);
    close(pipe_hijo2[0]);
    close(pipe_hijo1[0]);
    
    /*
     * Esperamos a que termine el Primer hijo
     * cuando el Primer hijo termina
     * significa que todos sus descendientes ya terminaron también
     */
    wait(NULL);
    
    /*
     * Mostramos los resultados que recibimos de cada proceso
     */
    printf("\n=== RESULTADOS (Proceso Padre) ===\n");
    printf("sumaA (fichero00): %d\n", sumaA);
    printf("sumaB (fichero01): %d\n", sumaB);
    printf("Suma total: %d\n", suma_total);
    printf("Verificación (sumaA + sumaB): %d\n", sumaA + sumaB);
    
    /*
     * Liberamos la memoria que asignamos al inicio
     * Es importante hacer esto para evitar memory leaks
     */
    free(arreglo1);
    free(arreglo2);
    
    printf("\n=== Programa finalizado ===\n");
    
    return 0;
}
