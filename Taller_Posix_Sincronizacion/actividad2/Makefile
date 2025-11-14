# ==============================================================
# Makefile para Actividad 2: Sincronización con Threads POSIX
# Pontificia Universidad Javeriana - Sistemas Operativos
# Autores: Juan Diego Ariza, Diego Mendoza
# Docente: J Corredor
# ==============================================================
CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lpthread

# Ejecutables
TARGET1 = posixSincro
TARGET2 = concurrencia

# Compilar ambos programas
all: posixSincro concurrencia

posixSincro: posixSincro.c
	$(CC) $(CFLAGS) -o posixSincro posixSincro.c $(LDFLAGS)

concurrencia: concurrencia.c
	$(CC) $(CFLAGS) -o concurrencia concurrencia.c $(LDFLAGS)

# Limpiar archivos compilados
clean:
	rm -f posixSincro concurrencia *.o
