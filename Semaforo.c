//
// Created by fedenote on 3/12/17.
//

#include "Semaforo.h"

int crearSemaforo ( const char* nombre, int id, const int valorInicial ) {
    key_t clave = ftok ( nombre, id);
    int idSem = semget ( clave, 1, 0666 | IPC_CREAT );
    if(idSem < 0) {
        return idSem;
    }
    int status = inicializar (idSem, valorInicial);
    if(status < 0) {
        return status;
    }
    return idSem;
}

int inicializar (int idSem, int valorInicial) {

    union semun {
        int val;
        /* Value for SETVAL */
        struct semid_ds *buf;
        /* Buffer for IPC_STAT, IPC_SET */
        unsigned short *array; /* Array for GETALL, SETALL */
        struct seminfo *__buf; /* Buffer for IPC_INFO(Linux specific)*/
    } init;
    
    init.val = valorInicial;
    int resultado = semctl ( idSem, 0, SETVAL, init );
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "El semaforo %d fallo al inicializar valor %d", idSem, valorInicial);
        perror(buffer);
    }
    return resultado;
}

int getSemaforo(int id, const char* path) {
    key_t clave;
    clave = ftok(path, id);
    int result = (semget(clave, 1, 0660));
    if(result < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "El semaforo con ID %d y path %s fallo en el get", id, path);
        perror(buffer);
    }
    return  result;
}

int p (const int semaforo) {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = -1;	// restar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( semaforo, &operacion, 1 )                                    ;
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Error al resta en el semaforo %d", semaforo);
        perror(buffer);
    }
    return resultado;
}

int v (const int semaforo) {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = 1;	// sumar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( semaforo, &operacion, 1 );
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Error al sumar en el semaforo %d", semaforo);
        perror(buffer);
    }
    return resultado;
}

void eliminarSemaforo (const int semaforo) {
    int resultado = semctl ( semaforo, 0, IPC_RMID );
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer),"Error al querer eliminar semaforo %d", semaforo);
        perror(buffer);
    }
}