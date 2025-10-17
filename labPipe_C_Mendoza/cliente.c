/******************************************************************************
* Pontificia Universidad Javeriana
* Autor: Diego Mendoza
* Materia: Sistemas Operativos
* Tema: Implementación de Productor Consumidor a parir de Pipe Named
* Descripción
*    - Archivo cliente que actúa como el Productor dentro del modelo
*    Productor-Consumidor utilizando Pipe
*    - Este programa se encarga de generar los datos que serán enviados
*    al proceso servidor (consumidor) a través del pipe
*******************************************************************************/

#include <stdio.h>	// Para printf, perror
#include <stdlib.h>	// Para exit
#include <string.h>	// Para strlen, strcpy, strcmp
#include <unistd.h>	// Para read, write, close
#include <fcntl.h>	// Para open
#include <sys/stat.h>	// Para mkfifo
#include <sys/types.h>	// Tipos básicos del sistema

#define FIFO_FILE "/tmp/PIPE_COMUNICATOR"

int main() {
   int fd;
   int end_process;	// Controla si se debe terminar el bucle
   int stringlen;	// Longitud de la cadena escrita por el usuario
   int read_bytes;	// Número de bytes leídos del FIFO
   char readbuf[80];	// Buffer para enviar/recibir mensajes
   char end_str[5];	// Palabra clave "end" para finalizar

   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

   // Abrir el FIFO en modo lectura/escritura
   fd = open(FIFO_FILE, O_RDWR);
   if (fd == -1) {
      perror("Error al abrir FIFO");
      exit(1);
   }

   strcpy(end_str, "end"); // Palabra clave de finalización

   // Ciclo principal para enviar mensajes al servidor
   while (1) {
      // Pedir al usuario una cadena
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);

      // Eliminar salto de línea al final
      stringlen = strlen(readbuf);
      if (stringlen > 0 && readbuf[stringlen - 1] == '\n')
         readbuf[stringlen - 1] = '\0'; // eliminar salto de línea

      // Comparar si el usuario escribió "end"
      end_process = strcmp(readbuf, end_str);

      if (end_process != 0) {
	// Enviar cadena al servidor
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                readbuf, (int)strlen(readbuf));

	// Esperar respuesta del servidor
         read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
         if (read_bytes > 0) {
            readbuf[read_bytes] = '\0';
            printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n",
                   readbuf, (int)strlen(readbuf));
         } else {
            perror("Error al leer del FIFO");
         }
      } else {
	// Si el usuario escribe "end", se envía y termina el proceso
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}

