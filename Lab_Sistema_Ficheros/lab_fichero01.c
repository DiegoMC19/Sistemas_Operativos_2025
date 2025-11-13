/********************************************************************
*
*	- PONTIFICIA UNIVERSIDAD JAVERIANA
*
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Tema: Sistema de Ficheros
* Descripción: Lee el contenido completo de un archivo llamado
* 	       "readme.txt" y lo muestra en consola línea por línea.
*
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>

int main() {
    // Nombre del archivo a leer
    char *filename = "readme.txt";
    
    // Abrir el archivo en modo lectura
    FILE *fp = fopen(filename, "r");
    
    // Verificar si el archivo se abrió correctamente
    if (fp == NULL) {
        printf("Error: could not open file %s", filename);
        return 1;
    }
    
    // Mostrar encabezado decorativo
    printf("\n ================================\n");
    printf("\n LECTURA DE FICHERO readme.txt\n");
    printf("\n ================================\n");
    
    // Configurar el tamaño máximo de cada línea
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    
    // Leer y mostrar cada línea del archivo
    while (fgets(buffer, MAX_LENGTH, fp))
        printf("%s", buffer);
    
    // Cerrar el archivo
    fclose(fp);
    
    return 0;
}
