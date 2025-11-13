/**************************************************************************
 * 
 *  - PONTIFICIA UNIVERSIDAD JAVERIANA
 * 
 * Autor: Diego Mendoza
 * Materia: Sistemas Operativos
 * Tema: Sistema de Ficheros
 * Descripción: Lee de teclado el nombre de un directorio y muestra en
 *              pantalla el nombre y el tamaño de los ficheros que contiene.
 *
 **************************************************************************/

#include <sys/types.h>  // Define tipos de datos usados en llamadas al sistema
#include <sys/stat.h>   // Proporciona la estructura stat y funciones para obtener información de archivos
#include <stdio.h>      // Funciones de entrada/salida estándar (printf, fgets)
#include <string.h>     // Funciones para manipulación de cadenas (strlen, strcpy, strcat)
#include <dirent.h>     // Funciones para trabajar con directorios (opendir, readdir, closedir)

int main() {
    DIR *d;
    char nomdir[90], nomfich[90];
    struct stat datos;
    struct dirent *direc;
    
    // Solicitar al usuario el nombre del directorio
    printf("Introduzca el Nombre de un Directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin);
    
    // Eliminar el salto de línea del nombre ingresado
    nomdir[strlen(nomdir) - 1] = '\0';
    
    // Intentar abrir el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("El directorio no existe\n");
        return -1;
    }
    
    // Leer cada entrada del directorio
    while ((direc = readdir(d)) != NULL) {
        // Copiar el nombre del directorio
        strcpy(nomfich, nomdir);
        
        // Agregar la barra separadora
        strcat(nomfich, "/");
        
        // Agregar el nombre del archivo
        strcat(nomfich, direc->d_name);
        
        // Obtener información del archivo
        stat(nomfich, &datos);
        
        // Si es un archivo regular, mostrar nombre y tamaño
        if (S_ISREG(datos.st_mode))
            printf("Nombre: %s\t Tamaño: %d\n", direc->d_name, datos.st_size);
    }
    
    // Fin del While 
    closedir(d);
    
    return 0;
}
