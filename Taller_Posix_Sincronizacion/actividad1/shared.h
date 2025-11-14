// Encabezado compartido
#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER 5

typedef struct {
    int bus[BUFFER];
    int entrada;
    int salida;
} compartir_datos;

#endif
