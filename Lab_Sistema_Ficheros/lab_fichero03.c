/**************************************************************************
 * 
 *  - PONTIFICIA UNIVERSIDAD JAVERIANA
 * 
 * Autor: Diego Mendoza
 * Materia: Sistemas Operativos
 * Docente: J Corredor
 * Tema: Sistema de Ficheros
 * Descripción: Lee de teclado el nombre de un directorio y muestra
 *              información detallada de los ficheros: nombre, permisos,
 *              tipo, modo y fecha de acceso de archivos recientes.
 *
 **************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int er;
    char nomdir[100], nomfich[100], resp[30];
    struct stat atr;
    DIR *d;
    struct dirent *rdi;
    time_t fecha;
    
    // Solicitar al usuario el nombre del directorio
    printf("Nombre directorio\n");
    fgets(nomdir, sizeof(nomdir), stdin);
    
    // Eliminar el salto de línea del nombre del directorio
    nomdir[strlen(nomdir) - 1] = '\0';
    
    // Obtener la fecha y hora actual
    fecha = time(&fecha);
    
    // Intentar abrir el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("No existe ese directorio \n");
        return -1;
    } else {
        // Leer cada entrada del directorio
        while ((rdi = readdir(d)) != NULL) {
            // Filtrar los directorios "." y ".."
            if ((strcmp(rdi->d_name, ".") != 0) && (strcmp(rdi->d_name, "..") != 0)) {
                // Construir la ruta completa del archivo
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/");
                strcat(nomfich, rdi->d_name);
                
                // Mostrar el nombre del fichero
                printf("fichero :%s:", nomfich);
                
                // Obtener información del archivo
                er = stat(nomfich, &atr);
                
                // Mostrar los permisos del archivo
                printf("modo :%o:", atr.st_mode);
                
                // Verificar si tiene permiso de lectura para el propietario
                if ((atr.st_mode & 0400) != 0)
                    printf(" permiso R para propietario\n");
                else
                    printf(" No permiso R para propietario\n");
                
                // Verificar si es un directorio
                if (S_ISDIR(atr.st_mode))
                    printf(" Es un directorio \n");
                
                // Verificar si es un archivo regular
                if (S_ISREG(atr.st_mode)) {
                    // Mostrar archivos modificados en los últimos 10 días
                    if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime) {
                        printf("FICHERO:%s: fecha acceso %s, en sgdos %d\n",
                               rdi->d_name,
                               ctime(&atr.st_mtime), atr.st_mtime);
                    }
                }
            }
        }
    }
    
    // Cerrar el directorio
    closedir(d);
    
    return 0;
}
/* CONCLUSIONES
- Este programa amplia un poco más la informacioón del lab02, mostrando permisos, fechas de acceso, etc.
- Permite tener un mayor control de los archivos mediante datos como fechas de modificación.
*/
