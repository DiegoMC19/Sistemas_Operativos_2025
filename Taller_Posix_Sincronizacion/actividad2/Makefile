# ==============================================================
# Makefile para Actividad 2: Sincronización con Threads POSIX
# Pontificia Universidad Javeriana - Sistemas Operativos
# Autores: Juan Diego Ariza, Diego Mendoza
# Docente: J Corredor
# ==============================================================
GCC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lpthread

PROGRAMAS = posixSincro concurrencia

all: $(PROGRAMAS)

posixSincro:
	$(GCC) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

concurrencia:
	$(GCC) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

clean:
	rm -f $(PROGRAMAS) *.o
