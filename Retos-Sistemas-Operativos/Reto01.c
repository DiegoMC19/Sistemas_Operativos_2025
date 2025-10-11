/***************************************************
*          Pontificia Universidad Javeriana        *
****************************************************
* Autor: Diego Mendoza                             *
* Fecha: 21 de agosto 2025                         *
* Docente: J. Corredor                             *
***************************************************/

#include <stdio.h>   // Librería estándar de entrada y salida
#include <string.h>  // Librería para manejo de cadenas (strcpy)
#include <stdlib.h>  // Librería para gestión de memoria dinamica
int main() {
    char *p;        // Se declara un puntero a char
    char *q = NULL; // Se declara un puntero a char inicializado en NULL
    //Imprime el contenido de p 
    printf("Address of p  %s\n", p);

    //Copia una cadena en la dirección a la que apunta "p"
    strcpy(p, "Helo, I'm the best in Operating Systems!!!");

    //Muestra un mensaje antes de copiar en "q"
    printf("About to copy \"Goodbye\" to q \n");

    //Copia una cadena en la dirección a la que apunta q
    strcpy(q, "Godbye");

    //Imprime un mensaje indicando que se pudo copiar bien
    printf("String copied\n");

    //Imprime el contenido de "q" como cadena
    printf("%s\n", q);
    return 0; // Fin del programa
}
