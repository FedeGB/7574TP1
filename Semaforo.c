//
// Created by fedenote on 3/12/17.
//

#include "Semaforo.h"


semaforo_st crearSemaforo ( const char* nombre, const int valorInicial ) {
    semaforo_st semaforo;
    semaforo.valorInicial = valorInicial;
    key_t clave = ftok ( nombre, 'a' );
    int idSem = semget ( clave, 1, 0666 | IPC_CREAT );
    int status = inicializar (idSem, valorInicial);
    if(status < 0) {
        return 0;
    }
    semaforo.id = idSem;
    return semaforo;
}

int inicializar (int idSem, int valorInicial) {

    union semnum {
        int val;
        struct semid_ds* buf;
        ushort* array;
    };

    semnum init;
    init.val = valorInicial;
    int resultado = semctl ( idSem, 0, SETVAL, init );
    return resultado;
}

int p (const semaforo_st semaforo) {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = -1;	// restar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( semaforo.id, &operacion, 1 );
    if(resultado < 0) {
        perror("Error al sumar en el semaforo %d", semaforo.id);
    }
    return resultado;
}

int v (const semaforo_st semaforo) {

    struct sembuf operacion;

    operacion.sem_num = 0;	// numero de semaforo
    operacion.sem_op  = 1;	// sumar 1 al semaforo
    operacion.sem_flg = SEM_UNDO;

    int resultado = semop ( semaforo.id, &operacion, 1 );
    if(resultado < 0) {
        perror("Error al restar en el semaforo %d", semaforo.id);
    }
    return resultado;
}

void eliminar (const semaforo_st semaforo) {
    int resultado = semctl ( semaforo.id, 0, IPC_RMID );
    if(resultado < 0) {
        perror("Error al querer eliminar semaforo %d", semaforo.id);
    }
}