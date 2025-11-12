/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autores: Juan Diego Ariza, Diego Mendoza
 * Materia: Sistemas Operativos
 * Taller 03 - Actividad 1: Productor-Consumidor con Semáforos POSIX
 * 
 * Descripción:
 * Este programa implementa el CONSUMIDOR del problema clásico productor-consumidor.
 * Utiliza semáforos con nombre de POSIX para sincronización entre procesos.
 *********************************************************************************************/

#include "shared.h"

int main() {
    // Abrir semáforos existentes (creados por el productor)
    sem_t *vacio = sem_open("/vacio", 0);
    sem_t *lleno = sem_open("/lleno", 0);
    
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open - Asegúrate de ejecutar el productor primero");
        exit(EXIT_FAILURE);
    }

    // Abrir memoria compartida existente
    int fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644);
    if (fd_compartido < 0) {
        perror("shm_open - Asegúrate de ejecutar el productor primero");
        exit(EXIT_FAILURE);
    }

    // Mapear la memoria compartida al espacio de direcciones del proceso
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), 
                                       PROT_READ | PROT_WRITE, 
                                       MAP_SHARED, fd_compartido, 0);
    
    // Inicializar el índice de salida
    compartir->salida = 0;

    printf("=== CONSUMIDOR INICIADO ===\n");
    printf("Consumiendo 10 elementos...\n\n");

    // Ciclo de consumo: consumir 10 elementos
    for (int i = 1; i <= 10; i++) {
        // Esperar si el buffer está vacío 
        sem_wait(lleno);
        
        // Extraer elemento del buffer
        int item = compartir->bus[compartir->salida];
        printf("Consumidor: Consume %d de posición %d\n", item, compartir->salida);
        
        // Avanzar el índice de salida 
        compartir->salida = (compartir->salida + 1) % BUFFER;
        
        // Señalar que hay un espacio más vacío
        sem_post(vacio);
        
        sleep(2);  // Simular tiempo de consumo 
    }

    printf("\n=== CONSUMIDOR FINALIZADO ===\n");

    // Limpieza de recursos
    munmap(compartir, sizeof(compartir_datos));
    close(fd_compartido);
    sem_close(vacio);
    sem_close(lleno);
    
    return 0;
}
