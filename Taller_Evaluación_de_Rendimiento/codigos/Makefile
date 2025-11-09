# **************************************************************
# * Nombres: Diego Mendoza, Juan Diego Ariza
# * Materia: Sistemas Operativos
# * Profesor: John Corredor, Ph.D.
# * Universidad: Pontificia Universidad Javeriana
# * Fecha: Noviembre 11 de 2025
# * Tema: Taller de Evaluación de Rendimiento
# * Descripción: Compilación de programas de multiplicación
# *              de matrices con diferentes paradigmas de
# *              paralelización (OpenMP, Pthreads, Fork)
# *************************************************************

# Definir el compilador a usar
GCC = gcc

# Flags comunes para todos los programas
CFLAGS = -lm

# Flags específicos para OpenMP
FOPENMP = -fopenmp -O3

# Flags para hilos POSIX (Pthreads)
POSIX = -lpthread

# Lista de todos los programas a compilar
PROGS = mmClasicaOpenMP mmClasicaPosix mmClasicaFork mmFilasOpenMP

all: $(PROGS)

mmClasicaOpenMP:
	$(GCC) $(CFLAGS) $@.c -o $@ $(FOPENMP)

mmClasicaPosix:
	$(GCC) $(CFLAGS) $@.c -o $@ $(POSIX)

mmClasicaFork:
	$(GCC) $(CFLAGS) $@.c -o $@

mmFilasOpenMP:
	$(GCC) $(CFLAGS) $@.c -o $@ $(FOPENMP)

# Elimina todos los ejecutables y archivos de datos generados
clean:
	rm -f $(PROGS) *.dat
