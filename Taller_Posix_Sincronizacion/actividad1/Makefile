# ========================================================
# Makefile - Actividad 1: Productor-Consumidor
# Sistemas Operativos - Pontificia Universidad Javeriana
# Autores: Juan Diego Ariza, Diego Mendoza
# Docente: J Corredor
# =====================================================
CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lrt -lpthread

# Compilar ambos programas
all: producer consumer

producer: producer.c shared.h
	$(CC) $(CFLAGS) -o producer producer.c $(LDFLAGS)

consumer: consumer.c shared.h
	$(CC) $(CFLAGS) -o consumer consumer.c $(LDFLAGS)

# Limpiar archivos compilados
clean:
	rm -f producer consumer *.o
