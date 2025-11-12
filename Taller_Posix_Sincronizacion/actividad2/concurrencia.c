/*********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autores: Juan Diego Ariza, Diego Mendoza
 * Docente: J Corredor
 * Materia: Sistemas Operativos
 * Taller 03 - Actividad 2: Búsqueda de máximo con hilos concurrentes
 * 
 * Descripción:
 * Este programa busca el valor máximo en un vector usando múltiples hilos.
 * Cada hilo busca el máximo en una porción del vector, y luego se combinan
 * los resultados para encontrar el máximo global.
 *********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para pasar argumentos a los hilos
struct argHilos {
    int inicio;
    int fin;
    int *vector;
    int maxparcial;
};

typedef struct argHilos param_H;

/**
 * Función ejecutada por cada hilo
 * Busca el máximo en un rango del vector
 */
void *buscarMax(void *parametro) {
    param_H *argumentos = (param_H *)parametro;
    argumentos->maxparcial = argumentos->vector[argumentos->inicio];
    
    for (int i = argumentos->inicio; i < argumentos->fin; i++) {
        if (argumentos->vector[i] > argumentos->maxparcial) {
            argumentos->maxparcial = argumentos->vector[i];
        }
    }
    
    pthread_exit(0);
    return NULL;
}

/**
 * Función que coordina la búsqueda del máximo usando hilos
 */
int maximoValor(int *vec, int n, int nhilos) {
    pthread_t *hilos;
    param_H *parametros;
    int elementosPorHilo, inicio, fin;
    int maximo;
    
    // Asignar memoria para los hilos y parámetros
    hilos = (pthread_t *)malloc(sizeof(pthread_t) * nhilos);
    parametros = (param_H *)malloc(sizeof(param_H) * nhilos);
    
    // Calcular cuántos elementos procesa cada hilo
    elementosPorHilo = n / nhilos;
    
    printf("\n=== BÚSQUEDA DE MÁXIMO CON %d HILOS ===\n", nhilos);
    printf("Vector de tamaño: %d\n", n);
    printf("Elementos por hilo: %d\n\n", elementosPorHilo);
    
    // Crear y lanzar los hilos
    for (int i = 0; i < nhilos; i++) {
        inicio = i * elementosPorHilo;
        
        // El último hilo procesa los elementos restantes
        if (i == nhilos - 1) {
            fin = n;
        } else {
            fin = (i + 1) * elementosPorHilo;
        }
        
        // Configurar parámetros para el hilo
        parametros[i].inicio = inicio;
        parametros[i].fin = fin;
        parametros[i].vector = vec;
        parametros[i].maxparcial = 0;
        
        // Crear el hilo
        if (pthread_create(&hilos[i], NULL, buscarMax, &parametros[i]) != 0) {
            fprintf(stderr, "Error creando hilo %d\n", i);
            exit(1);
        }
        
        printf("Hilo %d: buscando en rango [%d, %d)\n", i, inicio, fin);
    }
    
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < nhilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    // Encontrar el máximo global entre los máximos parciales
    maximo = parametros[0].maxparcial;
    printf("\n--- Resultados parciales ---\n");
    printf("Hilo 0: máximo parcial = %d\n", parametros[0].maxparcial);
    
    for (int i = 1; i < nhilos; i++) {
        printf("Hilo %d: máximo parcial = %d\n", i, parametros[i].maxparcial);
        if (parametros[i].maxparcial > maximo) {
            maximo = parametros[i].maxparcial;
        }
    }
    
    // Liberar memoria
    free(hilos);
    free(parametros);
    
    return maximo;
}

int main(int argc, char *argv[]) {
    FILE *fichero;
    int n, nhilos, i;
    int *vec;
    int ret, maximo;
    
    // Validar argumentos
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_datos> <numero_hilos>\n", argv[0]);
        fprintf(stderr, "Ejemplo: %s datos.txt 4\n", argv[0]);
        exit(-1);
    }
    
    // Abrir archivo de datos
    fichero = fopen(argv[1], "r");
    if (fichero == NULL) {
        perror("No se puede abrir fichero");
        exit(-2);
    }
    
    // Leer tamaño del vector
    ret = fscanf(fichero, "%d", &n);
    if (ret != 1) {
        fprintf(stderr, "No se puede leer tamaño\n");
        exit(-3);
    }
    
    // Obtener número de hilos
    nhilos = atoi(argv[2]);
    
    if (nhilos <= 0 || nhilos > n) {
        fprintf(stderr, "Número de hilos inválido (debe ser entre 1 y %d)\n", n);
        exit(-4);
    }
    
    // Asignar memoria para el vector
    vec = malloc(sizeof(int) * n);
    
    // Leer elementos del vector
    for (i = 0; i < n; ++i) {
        ret = fscanf(fichero, "%d", &vec[i]);
        if (ret != 1) {
            fprintf(stderr, "No se puede leer elemento nro %d\n", i);
            fclose(fichero);
            free(vec);
            exit(-5);
        }
    }
    
    // Buscar el máximo usando hilos
    maximo = maximoValor(vec, n, nhilos);
    
    printf("\n=== RESULTADO FINAL ===\n");
    printf("Máximo: %d\n\n", maximo);
    
    // Limpieza
    fclose(fichero);
    free(vec);
    
    return 0;
}
