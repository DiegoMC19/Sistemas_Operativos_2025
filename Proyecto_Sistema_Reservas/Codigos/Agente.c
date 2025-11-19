/******************************************************************************
 * Nombres:     Diego Alejandro Mendoza Cruz
 *              Juan Diego Ariza Lopez
 * Materia:     Sistemas Operativos
 * Profesor:    John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha:       Noviembre/2025
 * Tema:        Proyecto 2025-30: Sistema de Reservas de Parque
 * Taller:      Simulación de Sistema de Reservas
 * Descripción: Cliente que solicita reservas al Controlador mediante
 *              named pipes. Lee solicitudes desde un archivo CSV y
 *              envía peticiones de reserva para grupos familiares.
 *              Recibe respuestas del servidor (aprobadas, reprogramadas
 *              o negadas).
 *****************************************************************************/


#include "Common.h"

// Variables globales
char nombre_agente[MAX_NOMBRE]; // Nombre identificador de este agente
char file_solicitud[MAX_NOMBRE]; // Ruta del archivo CSV con solicitudes
char pipe_recibe[MAX_NOMBRE]; // Pipe del controlador (compartido)
char pipe_respuesta[MAX_NOMBRE]; // Pipe único de este agente (recibir)
int fd_pipe_respuesta;    	// Descriptor del pipe de respuesta
int hora_actual_sim = 0;	// Hora actual de la simulación

// Prototipos de funciones 
void registrarse_con_controlador();
void procesar_archivo_solicitudes();
void enviar_solicitud(const char *nombre_familia, int hora, int personas);
void recibir_respuesta();

// Función principal
int main(int argc, char *argv[]) {
    // Validar número de argumentos
    if (argc != 7) {
        fprintf(stderr, "Uso: %s -s nombre -a fileSolicitud -p pipeRecibe\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Procesar argumentos en cualquier orden
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-s") == 0) {
            strcpy(nombre_agente, argv[i + 1]);
        } else if (strcmp(argv[i], "-a") == 0) {
            strcpy(file_solicitud, argv[i + 1]);
        } else if (strcmp(argv[i], "-p") == 0) {
            strcpy(pipe_recibe, argv[i + 1]);
        }
    }
    
    // Mostrar información inicial
    printf("=== AGENTE DE RESERVA: %s ===\n", nombre_agente);
    printf("Archivo de solicitudes: %s\n", file_solicitud);
    printf("==============================\n\n");

    // Crear nombre único para pipe de respuesta
    snprintf(pipe_respuesta, MAX_NOMBRE, "/tmp/pipe_%s_%d", nombre_agente, getpid());

    // Crear pipe de respuesta
    unlink(pipe_respuesta);
    if (mkfifo(pipe_respuesta, 0666) == -1) {
        error_exit("Error al crear pipe de respuesta");
    }

    // Registrarse con el controlador
    registrarse_con_controlador();

    // Procesar archivo de solicitudes
    procesar_archivo_solicitudes();

    // Limpieza
    close(fd_pipe_respuesta);
    unlink(pipe_respuesta);

    printf("\nAgente %s termina.\n", nombre_agente);
    return 0;
}

// Registrarse con el controlador
void registrarse_con_controlador() {
    // Abrir pipe del controlador
    int fd_pipe_controlador = open(pipe_recibe, O_WRONLY);
    if (fd_pipe_controlador == -1) {
        error_exit("Error al conectar con el controlador");
    }

    // Preparar y enviar mensaje de registro
    MensajeRegistro msg;
    msg.tipo = MSG_REGISTRO;
    strcpy(msg.nombre_agente, nombre_agente);
    strcpy(msg.pipe_respuesta, pipe_respuesta);

    if (write(fd_pipe_controlador, &msg, sizeof(msg)) == -1) {
        close(fd_pipe_controlador);
        error_exit("Error al enviar registro");
    }

    close(fd_pipe_controlador);

    // Abrir pipe de respuesta para lectura
    fd_pipe_respuesta = open(pipe_respuesta, O_RDONLY);
    if (fd_pipe_respuesta == -1) {
        error_exit("Error al abrir pipe de respuesta");
    }

    // Esperar respuesta del controlador
    MensajeRespuesta resp;
    if (read(fd_pipe_respuesta, &resp, sizeof(resp)) > 0) {
        hora_actual_sim = resp.hora_asignada;
        printf("  Registro exitoso\n");
        printf("  %s\n\n", resp.mensaje);
    } else {
        error_exit("Error al recibir confirmación de registro");
    }
}

// Procesar archivo de solicitudes
void procesar_archivo_solicitudes() {
    // Abrir archivo de solicitudes
    FILE *archivo = fopen(file_solicitud, "r");
    if (archivo == NULL) {
        error_exit("Error al abrir archivo de solicitudes");
    }

    char linea[MAX_LINEA];
    int num_solicitud = 0;
    // Procesar cada línea del archivo
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        num_solicitud++;
        
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = 0;
        
        // Procesar la línea (formato: familia,hora,personas)
        char nombre_familia[MAX_NOMBRE];
        int hora, personas;
        // Lee hasta encontrar una coma 
        if (sscanf(linea, "%[^,],%d,%d", nombre_familia, &hora, &personas) != 3) {
            printf("  Línea %d inválida: %s\n", num_solicitud, linea);
            continue;
        }

        // Validar que la hora no sea anterior a la actual
        if (hora < hora_actual_sim) {
            printf("️  Solicitud #%d omitida: hora %d:00 ya pasó (actual: %d:00)\n", 
                   num_solicitud, hora, hora_actual_sim);
            sleep(2);
            continue;
        }

        // Enviar solicitud al controlador
        enviar_solicitud(nombre_familia, hora, personas);

        // Esperar 2 segundos antes de la siguiente solicitud
        sleep(2);
    }

    fclose(archivo);
}

// Enviar solicitud al controlador
void enviar_solicitud(const char *nombre_familia, int hora, int personas) {
    // Mostrar información de la solicitud
    printf("\n ENVIANDO SOLICITUD:\n");
    printf("   Familia: %s\n", nombre_familia);
    printf("   Hora: %d:00\n", hora);
    printf("   Personas: %d\n", personas);

    // Abrir pipe del controlador para modo escritura
    int fd_pipe_controlador = open(pipe_recibe, O_WRONLY);
    if (fd_pipe_controlador == -1) {
        printf("    Error al conectar con el controlador\n");
        return;
    }

    // Preparar mensaje de solicitud 
    MensajeSolicitud msg;
    msg.tipo = MSG_SOLICITUD;
    strcpy(msg.nombre_agente, nombre_agente);
    strcpy(msg.nombre_familia, nombre_familia);
    msg.hora_solicitada = hora;
    msg.num_personas = personas;

    // Enviar solicitud
    if (write(fd_pipe_controlador, &msg, sizeof(msg)) == -1) {
        printf("    Error al enviar solicitud\n");
        close(fd_pipe_controlador);
        return;
    }

    close(fd_pipe_controlador);

    // Esperar y recibir respuesta
    recibir_respuesta();
}

// Recibir respuesta del controlador
void recibir_respuesta() {
    MensajeRespuesta resp;
    
    // Leer respuesta del pipe
    ssize_t bytes = read(fd_pipe_respuesta, &resp, sizeof(resp));
    
    if (bytes > 0) {
        printf("\n RESPUESTA RECIBIDA:\n");
        
        // Mostrar respuesta según el tipo
        switch (resp.tipo_respuesta) {
            case RESP_OK:
                printf("    %s\n", resp.mensaje);
                break;
                
            case RESP_REPROGRAMADA:
                printf("    %s\n", resp.mensaje);
                break;
                
            case RESP_NEGADA_EXTEMPORANEA:
            case RESP_NEGADA_SIN_CUPO:
            case RESP_NEGADA_EXCEDE_AFORO:
                printf("    %s\n", resp.mensaje);
                break;
        }
    } else {
        printf("     Error al recibir respuesta\n");
    }
}
