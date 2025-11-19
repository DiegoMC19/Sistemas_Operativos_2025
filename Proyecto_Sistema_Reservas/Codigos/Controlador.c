/******************************************************************************
 * Nombres:     Diego Alejandro Mendoza Cruz
 *              Juan Diego Ariza Lopez
 * Materia:     Sistemas Operativos
 * Profesor:    John Corredor, Ph.D.
 * Universidad: Pontificia Universidad Javeriana
 * Fecha:       Noviembre/2025
 * Tema:        Proyecto 2025-30: Sistema de Reservas de Parque
 * Taller:      Simulación de Sistema de Reservas
 * Descripción: Servidor que gestiona las reservas del parque mediante
 *              procesos, hilos (pthread) y comunicación por named pipes.
 *              Controla el aforo máximo por hora y simula el paso del
 *              tiempo para autorizar entradas y salidas de familias.
 *****************************************************************************/


#include "Common.h" // Definiciones compartidas del proyecto


// Variables globales
int hora_ini, hora_fin, seg_horas, total_aforo; // Hora inicial de la simulación (7-19), Hora final de la simulación (7-19), Segundos que dura 1 hora simulada, Máximo de personas permitidas por hora
char pipe_recibe[MAX_NOMBRE]; // Nombre del pipe para recibir mensajes
int hora_actual;  // Hora actual de la simulación
int fd_pipe_recibe;    // File descriptor del pipe de recepción

// Mutexes para sincronización entre hilos
pthread_mutex_t mutex_reservas = PTHREAD_MUTEX_INITIALIZER;  // Protege reservas[]
pthread_mutex_t mutex_hora = PTHREAD_MUTEX_INITIALIZER;      // Protege hora_actual

int simulacion_activa = 1;  // Flag para controlar el bucle de los hilos

// Estructuras de datos
ReservaPorHora reservas[24]; // Reservas por cada hora del día 
InfoAgente agentes[MAX_AGENTES]; 
int num_agentes = 0;

// Estadísticas del día 
int solicitudes_aceptadas = 0;       // Reservas aprobadas en hora solicitada
int solicitudes_reprogramadas = 0;   // Reservas asignadas en hora diferente
int solicitudes_negadas = 0;         // Reservas rechazadas

// Prototipos de funciones 

// Hilos principales
void *hilo_reloj(void *arg);
void *hilo_escucha(void *arg);

// Procesamiento de mensajes
void procesar_registro(MensajeRegistro *msg);
void procesar_solicitud(MensajeSolicitud *msg);

// Funciones auxiliares
int buscar_bloque_disponible(int num_personas, int *hora_encontrada);
void enviar_respuesta(const char *pipe_agente, MensajeRespuesta *resp);
void mostrar_estado_hora();
void avanzar_hora();
void imprimir_reporte_final();
void signal_handler(int sig);

// Función principal
int main(int argc, char *argv[]) {
    
    // Validar número de argumentos
    if (argc != 11) {
        fprintf(stderr, "Uso: %s -i horaIni -f horaFin -s segHoras -t total -p pipeRecibe\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Procesar argumentos en cualquier orden
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-i") == 0) {
            hora_ini = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-f") == 0) {
            hora_fin = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-s") == 0) {
            seg_horas = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-t") == 0) {
            total_aforo = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-p") == 0) {
            strcpy(pipe_recibe, argv[i + 1]);
        }
    }

    // Validar rangos de horas
    if (hora_ini < HORA_MIN || hora_ini > HORA_MAX || 
        hora_fin < HORA_MIN || hora_fin > HORA_MAX || 
        hora_ini >= hora_fin) {
        fprintf(stderr, "Error: Horas inválidas (rango permitido: %d-%d)\n", HORA_MIN, HORA_MAX);
        exit(EXIT_FAILURE);
    }

    // Mostrar configuración
    printf("=== CONTROLADOR DE RESERVAS ===\n");
    printf("Período de simulación: %d:00 - %d:00\n", hora_ini, hora_fin);
    printf("Aforo máximo por hora: %d personas\n", total_aforo);
    printf("Velocidad: 1 hora = %d segundos\n", seg_horas);
    printf("================================\n\n");

    // Inicializar hora actual
    hora_actual = hora_ini;

    // Inicializar arreglo de reservas (todas a cero)
    for (int i = 0; i < 24; i++) {
        reservas[i].personas_reservadas = 0;
        reservas[i].num_familias = 0;
    }

    // Configurar handler para SIGINT
    signal(SIGINT, signal_handler);

    // Crear named pipe para comunicación con agentes
    unlink(pipe_recibe); // Eliminar si existe de ejecución anterior
    if (mkfifo(pipe_recibe, 0666) == -1) {
        error_exit("Error al crear pipe nominal");
    }

    printf("Pipe '%s' creado. Esperando agentes...\n\n", pipe_recibe);

    // Abrir pipe para lectura (no bloqueante)
    fd_pipe_recibe = open(pipe_recibe, O_RDONLY | O_NONBLOCK);
    if (fd_pipe_recibe == -1) {
        error_exit("Error al abrir pipe para lectura");
    }

    // Crear hilos
    pthread_t tid_reloj, tid_escucha;

    // Hilo del reloj: simula el paso del tiempo
    if (pthread_create(&tid_reloj, NULL, hilo_reloj, NULL) != 0) {
        error_exit("Error al crear hilo del reloj");
    }

    // Hilo de escucha: recibe y procesa mensajes de agentes
    if (pthread_create(&tid_escucha, NULL, hilo_escucha, NULL) != 0) {
        error_exit("Error al crear hilo de escucha");
    }

    // Esperar a que terminen los hilos
    pthread_join(tid_reloj, NULL);
    pthread_join(tid_escucha, NULL);

    // Limpieza de recursos
    close(fd_pipe_recibe);
    unlink(pipe_recibe);

    // Imprimir reporte final
    imprimir_reporte_final();

    printf("\nControlador finalizado.\n");
    return 0;
}

// Hilo del reloj - simula el paso del tiempo
void *hilo_reloj(void *arg) {
    (void)arg; // Evitar warning de parámetro no usado
    
    // Mostrar estado inicial antes de empezar a avanzar
    sleep(seg_horas);
    mostrar_estado_hora();
    
    // Bucle principal del reloj
    while (simulacion_activa) {
        sleep(seg_horas);
        
        pthread_mutex_lock(&mutex_hora);
        if (hora_actual >= hora_fin) {
            simulacion_activa = 0;
            pthread_mutex_unlock(&mutex_hora);
            break;
        }
        pthread_mutex_unlock(&mutex_hora);

        // Avanzar una hora y mostrar cambios
        avanzar_hora();
    }
    
    return NULL;
}

// Hilo de escucha - recibe mensajes de los agentes
void *hilo_escucha(void *arg) {
    (void)arg; // Evitar warning
    
    char buffer[sizeof(MensajeSolicitud)];
    ssize_t bytes_leidos;

    while (simulacion_activa) {
        // Intentar leer del pipe
        bytes_leidos = read(fd_pipe_recibe, buffer, sizeof(buffer));
        
        if (bytes_leidos > 0) {
            // Identificar tipo de mensaje
            TipoMensaje *tipo = (TipoMensaje *)buffer;
            
            if (*tipo == MSG_REGISTRO) {
                MensajeRegistro *msg = (MensajeRegistro *)buffer;
                procesar_registro(msg);
            } else if (*tipo == MSG_SOLICITUD) {
                MensajeSolicitud *msg = (MensajeSolicitud *)buffer;
                procesar_solicitud(msg);
            }
        } else if (bytes_leidos == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
            break;
        }
        
        // Pequeña pausa para no saturar la CPU
        usleep(10000); // 10ms
    }
    
    return NULL;
}

// Procesar registro de un nuevo agente
void procesar_registro(MensajeRegistro *msg) {
    pthread_mutex_lock(&mutex_hora);
    
    if (num_agentes < MAX_AGENTES) {
        // Guardar información del agente
        strcpy(agentes[num_agentes].nombre, msg->nombre_agente);
        strcpy(agentes[num_agentes].pipe_respuesta, msg->pipe_respuesta);
        agentes[num_agentes].activo = 1;
        num_agentes++;
        
        log_mensaje("Agente '%s' registrado", msg->nombre_agente);
        
        // Preparar respuesta con hora actual
        MensajeRespuesta resp;
        resp.tipo_respuesta = RESP_OK;
        resp.hora_asignada = hora_actual;
        snprintf(resp.mensaje, MAX_LINEA, "Registrado. Hora actual: %d:00", hora_actual);
        
        // Enviar respuesta al agente
        enviar_respuesta(msg->pipe_respuesta, &resp);
    }
    
    pthread_mutex_unlock(&mutex_hora);
}

// Procesar solicitud de reserva
void procesar_solicitud(MensajeSolicitud *msg) {
    pthread_mutex_lock(&mutex_reservas);
    pthread_mutex_lock(&mutex_hora);
    
    MensajeRespuesta resp;
    
    // Mostrar información de la solicitud
    printf("\n--- SOLICITUD RECIBIDA ---\n");
    printf("Agente: %s\n", msg->nombre_agente);
    printf("Familia: %s\n", msg->nombre_familia);
    printf("Hora solicitada: %d:00\n", msg->hora_solicitada);
    printf("Personas: %d\n", msg->num_personas);
    
    // Validar que la hora solicitada no sea pasada
    if (msg->hora_solicitada < hora_actual) {
        printf("Resultado: NEGADA (hora extemporánea)\n");
        
        // Intentar reprogramar
        int hora_encontrada;
        if (buscar_bloque_disponible(msg->num_personas, &hora_encontrada)) {
            resp.tipo_respuesta = RESP_REPROGRAMADA;
            resp.hora_asignada = hora_encontrada;
            resp.num_personas = msg->num_personas;
            
            // Registrar la reserva en las 2 horas consecutivas
            for (int i = 0; i < DURACION_RESERVA; i++) {
                int hora_idx = hora_encontrada + i;
                reservas[hora_idx].personas_reservadas += msg->num_personas;
                strcpy(reservas[hora_idx].familias[reservas[hora_idx].num_familias], 
                       msg->nombre_familia);
                reservas[hora_idx].personas_por_familia[reservas[hora_idx].num_familias] = 
                    msg->num_personas;
                reservas[hora_idx].hora_entrada[reservas[hora_idx].num_familias] = 
                    hora_encontrada;
                reservas[hora_idx].num_familias++;
            }
            
            snprintf(resp.mensaje, MAX_LINEA, 
                     "Reserva reprogramada para %d:00 (%d personas)", 
                     hora_encontrada, msg->num_personas);
            solicitudes_reprogramadas++;
            printf("Reprogramada para: %d:00\n", hora_encontrada);
        } else {
            resp.tipo_respuesta = RESP_NEGADA_SIN_CUPO;
            snprintf(resp.mensaje, MAX_LINEA, "No hay cupo disponible. Intente otro día.");
            solicitudes_negadas++;
        }
    }
    // Validar que no exceda el aforo
    else if (msg->num_personas > total_aforo) {
        resp.tipo_respuesta = RESP_NEGADA_EXCEDE_AFORO;
        snprintf(resp.mensaje, MAX_LINEA, 
                 "El grupo excede el aforo máximo (%d personas)", total_aforo);
        solicitudes_negadas++;
        printf("Resultado: NEGADA (excede aforo)\n");
    }
    // Validar que la hora esté dentro del rango
    else if (msg->hora_solicitada > hora_fin || 
             msg->hora_solicitada + DURACION_RESERVA - 1 > hora_fin) {
        resp.tipo_respuesta = RESP_NEGADA_SIN_CUPO;
        snprintf(resp.mensaje, MAX_LINEA, "Hora fuera del período de operación.");
        solicitudes_negadas++;
        printf("Resultado: NEGADA (fuera de rango)\n");
    }
    // Verificar disponibilidad en las horas solicitadas
    else {
        int hay_cupo = 1;
        for (int i = 0; i < DURACION_RESERVA; i++) {
            int hora_idx = msg->hora_solicitada + i;
            if (reservas[hora_idx].personas_reservadas + msg->num_personas > total_aforo) {
                hay_cupo = 0;
                break;
            }
        }
        
        if (hay_cupo) {
            // Reserva aprobada
            resp.tipo_respuesta = RESP_OK;
            resp.hora_asignada = msg->hora_solicitada;
            resp.num_personas = msg->num_personas;
            
            // Registrar la reserva
            for (int i = 0; i < DURACION_RESERVA; i++) {
                int hora_idx = msg->hora_solicitada + i;
                reservas[hora_idx].personas_reservadas += msg->num_personas;
                strcpy(reservas[hora_idx].familias[reservas[hora_idx].num_familias], 
                       msg->nombre_familia);
                reservas[hora_idx].personas_por_familia[reservas[hora_idx].num_familias] = 
                    msg->num_personas;
                reservas[hora_idx].hora_entrada[reservas[hora_idx].num_familias] = 
                    msg->hora_solicitada;
                reservas[hora_idx].num_familias++;
            }
            
            snprintf(resp.mensaje, MAX_LINEA, 
                     "Reserva confirmada para %d:00 (%d personas)", 
                     msg->hora_solicitada, msg->num_personas);
            solicitudes_aceptadas++;
            printf("Resultado: APROBADA\n");
        } else {
            // No hay cupo, buscar alternativa
            int hora_encontrada;
            if (buscar_bloque_disponible(msg->num_personas, &hora_encontrada)) {
                resp.tipo_respuesta = RESP_REPROGRAMADA;
                resp.hora_asignada = hora_encontrada;
                resp.num_personas = msg->num_personas;
                
                // Registrar la reserva
                for (int i = 0; i < DURACION_RESERVA; i++) {
                    int hora_idx = hora_encontrada + i;
                    reservas[hora_idx].personas_reservadas += msg->num_personas;
                    strcpy(reservas[hora_idx].familias[reservas[hora_idx].num_familias], 
                           msg->nombre_familia);
                    reservas[hora_idx].personas_por_familia[reservas[hora_idx].num_familias] = 
                        msg->num_personas;
                    reservas[hora_idx].hora_entrada[reservas[hora_idx].num_familias] = 
                        hora_encontrada;
                    reservas[hora_idx].num_familias++;
                }
                
                snprintf(resp.mensaje, MAX_LINEA, 
                         "Reserva reprogramada para %d:00 (%d personas)", 
                         hora_encontrada, msg->num_personas);
                solicitudes_reprogramadas++;
                printf("Resultado: REPROGRAMADA para %d:00\n", hora_encontrada);
            } else {
                resp.tipo_respuesta = RESP_NEGADA_SIN_CUPO;
                snprintf(resp.mensaje, MAX_LINEA, "No hay cupo disponible. Intente otro día.");
                solicitudes_negadas++;
                printf("Resultado: NEGADA (sin cupo)\n");
            }
        }
    }
    
    printf("--------------------------\n");
    
    // Buscar pipe del agente y enviar respuesta
    for (int i = 0; i < num_agentes; i++) {
        if (strcmp(agentes[i].nombre, msg->nombre_agente) == 0) {
            enviar_respuesta(agentes[i].pipe_respuesta, &resp);
            break;
        }
    }
    
    pthread_mutex_unlock(&mutex_hora);
    pthread_mutex_unlock(&mutex_reservas);
}

// Buscar un bloque de 2 horas disponible
int buscar_bloque_disponible(int num_personas, int *hora_encontrada) {
    // Recorrer todas las horas posibles
    for (int hora = hora_actual; hora <= hora_fin - DURACION_RESERVA + 1; hora++) {
        int disponible = 1;

        // Verificar que ambas horas tengan cupo
        for (int i = 0; i < DURACION_RESERVA; i++) {
            if (reservas[hora + i].personas_reservadas + num_personas > total_aforo) {
                disponible = 0;
                break;
            }
        }
        if (disponible) {
            *hora_encontrada = hora;
            return 1; // Encontrado
        }
    }
    return 0;  // No hay cupo disponible
}

// Enviar respuesta a un agente
void enviar_respuesta(const char *pipe_agente, MensajeRespuesta *resp) {
    int fd = open(pipe_agente, O_WRONLY);
    if (fd != -1) {
        write(fd, resp, sizeof(MensajeRespuesta));
        close(fd);
    }
}

// Mostrar estado de la hora actual sin avanzar el reloj
void mostrar_estado_hora() {
    pthread_mutex_lock(&mutex_hora);
    pthread_mutex_lock(&mutex_reservas);
    
    printf("\n");
    printf("================================\n");
    printf("  HORA ACTUAL: %d:00\n", hora_actual);
    printf("================================\n");
    
    // Mostrar familias que entran en la hora actual
    if (reservas[hora_actual].num_familias > 0) {
        int hay_entradas = 0;
        for (int i = 0; i < reservas[hora_actual].num_familias; i++) {
            // Solo mostrar si es su hora de entrada
            if (reservas[hora_actual].hora_entrada[i] == hora_actual) {
                if (!hay_entradas) {
                    printf(" ENTRAN AL PARQUE:\n");
                    hay_entradas = 1;
                }
                printf("   - Familia %s (%d personas)\n", 
                       reservas[hora_actual].familias[i],
                       reservas[hora_actual].personas_por_familia[i]);
            }
        }
    }
    
    printf(" Ocupación actual: %d/%d personas\n", 
           reservas[hora_actual].personas_reservadas, total_aforo);
    printf("================================\n\n");
    
    pthread_mutex_unlock(&mutex_reservas);
    pthread_mutex_unlock(&mutex_hora);
}

// Avanzar el reloj una hora y mostrar cambios 
void avanzar_hora() {
    pthread_mutex_lock(&mutex_hora);
    pthread_mutex_lock(&mutex_reservas);
    
    hora_actual++;
    
    printf("\n");
    printf("================================\n");
    printf("  HORA ACTUAL: %d:00\n", hora_actual);
    printf("================================\n");
    
    // Mostrar familias que salen (terminan su reserva)
    if (hora_actual - DURACION_RESERVA >= hora_ini) {
        int hora_salida = hora_actual - DURACION_RESERVA;
        if (reservas[hora_salida].num_familias > 0) {
            int hay_salidas = 0;
            for (int i = 0; i < reservas[hora_salida].num_familias; i++) {
                // Solo mostrar salida si es su hora de entrada + 2
                if (reservas[hora_salida].hora_entrada[i] == hora_salida) {
                    if (!hay_salidas) {
                        printf(" SALEN DEL PARQUE:\n");
                        hay_salidas = 1;
                    }
                    printf("   - Familia %s (%d personas)\n", 
                           reservas[hora_salida].familias[i],
                           reservas[hora_salida].personas_por_familia[i]);
                }
            }
        }
    }
    
    // Mostrar familias que entran
    if (hora_actual <= hora_fin && reservas[hora_actual].num_familias > 0) {
        int hay_entradas = 0;
        for (int i = 0; i < reservas[hora_actual].num_familias; i++) {
            if (reservas[hora_actual].hora_entrada[i] == hora_actual) {
                if (!hay_entradas) {
                    printf(" ENTRAN AL PARQUE:\n");
                    hay_entradas = 1;
                }
                printf("   - Familia %s (%d personas)\n", 
                       reservas[hora_actual].familias[i],
                       reservas[hora_actual].personas_por_familia[i]);
            }
        }
    }
    
    printf(" Ocupación actual: %d/%d personas\n", 
           reservas[hora_actual].personas_reservadas, total_aforo);
    printf("================================\n\n");
    
    pthread_mutex_unlock(&mutex_reservas);
    pthread_mutex_unlock(&mutex_hora);
}

// Imprimir reporte final
void imprimir_reporte_final() {
    printf("\n");
    printf("========================================\n");
    printf("         REPORTE FINAL DEL DÍA         \n");
    printf("========================================\n\n");
    
    // Horas pico
    int max_personas = 0;
    for (int i = hora_ini; i <= hora_fin; i++) {
        if (reservas[i].personas_reservadas > max_personas) {
            max_personas = reservas[i].personas_reservadas;
        }
    }
    
    printf(" HORAS PICO (mayor ocupación):\n");
    for (int i = hora_ini; i <= hora_fin; i++) {
        if (reservas[i].personas_reservadas == max_personas) {
            printf("   - %d:00 con %d personas\n", i, max_personas);
        }
    }
    
    // Horas valle
    int min_personas = total_aforo;
    for (int i = hora_ini; i <= hora_fin; i++) {
        if (reservas[i].personas_reservadas < min_personas) {
            min_personas = reservas[i].personas_reservadas;
        }
    }
    
    printf("\n HORAS VALLE (menor ocupación):\n");
    for (int i = hora_ini; i <= hora_fin; i++) {
        if (reservas[i].personas_reservadas == min_personas) {
            printf("   - %d:00 con %d personas\n", i, min_personas);
        }
    }
    
    // Estadísticas de solicitudes
    printf("\n ESTADÍSTICAS DE SOLICITUDES:\n");
    printf("   Aceptadas en su hora: %d\n", solicitudes_aceptadas);
    printf("   Reprogramadas: %d\n", solicitudes_reprogramadas);
    printf("   Negadas: %d\n", solicitudes_negadas);
    printf("   Total: %d\n", 
           solicitudes_aceptadas + solicitudes_reprogramadas + solicitudes_negadas);
    
    printf("\n========================================\n");
}

// Manejador de señales
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\n\nRecibida señal de interrupción. Finalizando...\n");
        simulacion_activa = 0;
    }
}
