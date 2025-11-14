# ========================================================
# Makefile - Actividad 1: Productor-Consumidor
# Sistemas Operativos - Pontificia Universidad Javeriana
# Autores: Juan Diego Ariza, Diego Mendoza
# Docente: J Corredor
# =====================================================
GCC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lrt -lpthread

PROGRAMAS = producer consumer

all: $(PROGRAMAS)

producer:
	$(GCC) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

consumer:
	$(GCC) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

clean:
	rm -f $(PROGRAMAS) *.o
