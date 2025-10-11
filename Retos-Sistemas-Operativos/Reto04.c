/***************************************************
*          Pontificia Universidad Javeriana        *
****************************************************
* Autor: Diego Mendoza                             *
* Fecha: 21 de agosto 2025                         *
* Docente: J. Corredor                             *
***************************************************/
#include <stdio.h>  //Librería estándar de entrada y salida
#include <stdlib.h> //Para calloc() y free()

int main() {
    int n, i, *ptr, sum = 0; //Creación de variables tipo entero, entre ellas un puntero

    printf("Enter number of elements: "); //Pide numero de elementos a ingresar
    scanf("%d", &n);

    // Reservar memoria dinámica para n enteros
    ptr = (int*) calloc(n, sizeof(int));
    if (ptr == NULL) {
        printf("Error! memory not allocated.\n"); //Si no tiene memoria sale un error
        return 1; //Salida con error
    }
    printf("Enter elements:\n"); //Nos pide los elementos 
    for (i = 0; i < n; i++) {
        printf("Element %d: ", i + 1); //Nos muestra los numeros hasta el numero de elementos que especificamos antes  
        scanf("%d", ptr + i);   //Leer en la posición i
        sum += *(ptr + i);      //Acumular el valor leído
    }
    printf("Sum = %d\n", sum); //Muestra el resultado de la suma de los elementos digitados

    // Liberar memoria
    free(ptr);
    return 0;
}
