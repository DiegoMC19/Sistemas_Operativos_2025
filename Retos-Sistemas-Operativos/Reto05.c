/***************************************************
*          Pontificia Universidad Javeriana        *
****************************************************
* Autor: Diego Mendoza                             *
* Fecha: 21 de agosto 2025                         *
* Docente: J. Corredor                             *
***************************************************/
#include <stdio.h>  //Librería estándar de entrada y salida
#include <stdlib.h> //Para calloc() y free()

int main(){
 int *ptr, i, n1,n2; //Variables enteras, una de ellas es puntero
 printf("Enter size: ");// Solicitar tamaño inicial 
 scanf("%d", &n1); //Recibe el valor

 ptr = (int*) malloc(n1 * sizeof(int)); // Asignación dinámica de memoria
 printf("Addresses of previously alocated memory: "); 
 for(i=0;i<n1;i++)
     printf("\n\np = %p\n", ptr+i);

 printf("\n Enter the new size: "); // Solicitar nuevo tamaño
 scanf("%d", &n2);//Recibe el valor

 ptr = realloc(ptr, n2 * sizeof(int)); // Reasignar memoria con realloc
 printf("Addresses of newly allocated memory: "); //Mostrar las nuevas direcciones de memoria asignadas
 for(i=0;i<n2;i++)
      printf("\n\np = %p\n", ptr+i);

 free(ptr); //Liberar memoria
