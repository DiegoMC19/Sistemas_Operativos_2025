EJECUTABLE = ejecutar

all: $(EJECUTABLE)

$(EJECUTABLE): Fuente.o modulo.o
	gcc Fuente.o modulo.o -o $(EJECUTABLE) -lm

Fuente.o: Fuente.c Interfaz.h
	gcc -c Fuente.c

modulo.o: modulo.c Interfaz.h
	gcc -c modulo.c

clean:
	rm -f *.o $(EJECUTABLE)
