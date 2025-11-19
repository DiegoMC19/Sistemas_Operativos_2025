##############################################################################
# Nombres:     Diego Alejandro Mendoza Cruz
#              Juan Diego Ariza Lopez
# Materia:     Sistemas Operativos
# Profesor:    John Corredor, Ph.D.
# Universidad: Pontificia Universidad Javeriana
# Fecha:       Noviembre/2025
# Tema:        Proyecto 2025-30: Sistema de Reservas de Parque
# Taller:      Simulación de Sistema de Reservas
# Descripción: Script de automatización para compilación de programas
#              POSIX con soporte para hilos (pthread). Compila el
#              Controlador (servidor) y los Agentes (clientes) con sus
#              dependencias compartidas (Common.c).
##############################################################################


GCC = gcc
FLAGS = -lpthread
PROGRAMAS = Controlador Agente

Controlador:
	$(GCC) $@.c Common.c -o $@ $(FLAGS)

Agente:
	$(GCC) $@.c Common.c -o $@ $(FLAGS)

clean:
	$(RM) $(PROGRAMAS)
