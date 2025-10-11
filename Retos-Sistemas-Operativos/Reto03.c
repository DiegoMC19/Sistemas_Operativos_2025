/***************************************************
*          Pontificia Universidad Javeriana        *
****************************************************
* Autor: Diego Mendoza                             *
* Fecha: 21 de agosto 2025                         *
* Docente: J. Corredor                             *
***************************************************/
#include <stdio.h>   //Librería estándar de entrada y salida
#include <stdlib.h>  //Necesario para malloc y free

int main() {
    int *ptr;  //Variable puntero a entero

    //Reservamos memoria para 15 enteros
    ptr = (int *)malloc(15 * sizeof(*ptr)); 

    if (ptr != NULL) {  
        //Se asigna el valor 480 al 6to entero (posición 5, porque empieza en 0)
        *(ptr + 5) = 480;

        //Imprime el valor para comprobar
        printf("Value of the 6th integer is %d\n", *(ptr + 5));
    } else {
        printf("Error: no se pudo asignar memoria.\n"); //Impre error si no funciona de forma correcta
    }


    //Liberamos memoria
    free(ptr);
    return 0; /Finaliza el programa
}
