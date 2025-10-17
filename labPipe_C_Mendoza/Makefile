###########################################
# Makefile
# Script para automatización de compilación
###########################################

GCC = gcc
FLAGS =
PROGRAMAS = cliente servidor

all: $(PROGRAMAS)

cliente:
	$(GCC) $@.c -o $@ $(FLAGS)

servidor:
	$(GCC) $@.c -o $@ $(FLAGS)

clean:
	rm -f $(PROGRAMAS)

