/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autores: Juan Diego Ariza, Diego Mendoza
 * Docente: J Corredor
 * Materia: Sistemas Operativos
 * Taller 03 - Actividad 1: Productor-Consumidor con Semáforos POSIX
 * 
 * Descripción:
 * Este programa implementa el PRODUCTOR del problema clásico productor-consumidor.
 * Utiliza semáforos con nombre de POSIX para sincronización entre procesos.
 *********************************************************************************************/

#include "shared.h"

int main() {
    // Crear/abrir semáforos con nombre
    // vacio: inicializado con BUFFER (5 espacios vacíos)
    // lleno: inicializado con 0 (0 espacios ocupados)
    sem_t *vacio = sem_open("/vacio", O_CREAT, 0644, BUFFER);
    sem_t *lleno = sem_open("/lleno", O_CREAT, 0644, 0);
    
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Crear memoria compartida
    int shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // Establecer el tamaño de la memoria compartida
    ftruncate(shm_fd, sizeof(compartir_datos));

    // Mapear la memoria compartida al espacio de direcciones del proceso
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), 
                                       PROT_READ | PROT_WRITE, 
                                       MAP_SHARED, shm_fd, 0);
    
    // Inicializar el índice de entrada
    compartir->entrada = 0;

    printf("=== PRODUCTOR INICIADO ===\n");
    printf("Produciendo 10 elementos...\n\n");

    // Ciclo de producción: generar 10 elementos
    for (int i = 1; i <= 10; i++) {
        // Esperar si el buffer está lleno (sem_wait decrementa el semáforo)
        sem_wait(vacio);
        
        // Agregar elemento al buffer
        compartir->bus[compartir->entrada] = i;
        printf("Productor: Produce %d en posición %d\n", i, compartir->entrada);
        
        // Avanzar el índice de entrada 
        compartir->entrada = (compartir->entrada + 1) % BUFFER;
        
        // Señalar que hay un elemento más lleno
        sem_post(lleno);
        
        sleep(1);  // Simular tiempo de producción
    }

    printf("\n=== PRODUCTOR FINALIZADO ===\n");

    // Limpieza de recursos
    munmap(compartir, sizeof(compartir_datos));
    close(shm_fd);
    sem_close(vacio);
    sem_close(lleno);
    
    // Eliminar semáforos y memoria compartida
    sem_unlink("/vacio");
    sem_unlink("/lleno");
    shm_unlink("/memoria_compartida");
    
    return 0;
}
