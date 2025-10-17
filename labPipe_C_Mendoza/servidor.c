************************************************************************************
* Pontificia Universidad Javeriana
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Tema: Implementación de Productor Consumidor a parir de Pipe Named
* Descripción
*   - Archivo servidor que permite la creación y gestión del pipe nombrado
*   - Actúa como el proceso **productor**, enviando datos al cliente (consumidor).
*   - Se encarga de abrir el FIFO en modo escritura, enviar los mensajes y cerrar
*     la comunicación correctamente.
*   - Incluye control de errores y manejo de señales para finalizar el proceso
*     de forma segura.
************************************************************************************/


#include <stdio.h>    // Para printf, perror
#include <stdlib.h>   // Para exit
#include <string.h>   // Para srlen, memset
#include <unistd.h>   // Para read, write, close
#include <fcntl.h>    // Para open
#include <sys/stat.h> // Para mkfifo
#include <sys/types.h> // Para tipos del sistema

#define FIFO_FILE "/tmp/PIPE_COMUNICATOR"       

void reverse_string(char *);

int main() {
   int fd;
   char readbuf[80];	//Buffer donde se almacenan los mensajes recibidos
   char end[10];	// Palabra clave para terminar
   int to_end;		// Variable para verificar si se debe finalizar	
   int read_bytes;	 // Almacena el número de bytes leídos

   /* Crear el FIFO si no existe */
   mkfifo(FIFO_FILE, 0660);

   strcpy(end, "end");		// Se define la palabra clave de salida
   fd = open(FIFO_FILE, O_RDWR);  // Se abre el FIFO en modo lectura/escritura
   if (fd == -1) {
      perror("Error al abrir FIFO");
      exit(1);
   }

   printf("FIFOSERVER: Esperando mensajes...\n");

   // Bucle principal: se mantiene escuchando mensajes
   while (1) {
	// Leer datos desde el FIFO
      read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
      if (read_bytes <= 0) {
         perror("Error al leer del FIFO");
         break;		// Sale del bucle en caso de error
      }
	// Se añade el carácter nulo al final de la cadena recibida
      readbuf[read_bytes] = '\0';
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n",
             readbuf, (int)strlen(readbuf));

	// Si el cliente envía "end", se cierra el servidor
      to_end = strcmp(readbuf, end);
      if (to_end == 0) {
         printf("FIFOSERVER: End signal received. Closing...\n");
         close(fd);		// Cierra el descriptor del FIFO
         unlink(FIFO_FILE);	// Elimina el FIFO del sistema
         break;			// Sale del bucle principal
      }

      // Invertir la cadena recibida
      reverse_string(readbuf);

      // Mostrar y enviar la cadena invertida al cliente
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n",
             readbuf, (int)strlen(readbuf));

      write(fd, readbuf, strlen(readbuf));	// Enviar al cliente
      sleep(1); // Pequeña pausa para que el cliente alcance a leer
   }
   return 0;
}

/* Función que invierte una cadena de texto */
void reverse_string(char *str) {
   int first = 0;		// Indice inicial
   int last = strlen(str) - 1;	// Indice final
   char temp;			// Variable temporal para intercambio

 // Intercambia los caracteres desde los extremos hacia el centro
   while (first < last) {
      temp = str[first];
      str[first] = str[last];
      str[last] = temp;
      first++;
      last--;
   }
}
