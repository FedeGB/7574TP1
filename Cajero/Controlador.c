//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


int start(int* queues, pid_t* cajero, pid_t* middleware) {
    iniciarIPCs(queues);
    bool trabajador = iniciarTrabajadores(cajero, middleware, queues);
    if(trabajador) {
        return 1;
    } else {
        return 0;
    }
}


void iniciarIPCs(int* queues) {
    queues[0] = creamsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
    queues[1] = createSocket("127.0.0.1", 8082, false);
    queues[2] = queues[1];
    queues[3] = creamsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
    queues[4] = creamsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
    queues[5] = createSocket("", 0, false);
    queues[6] = creamsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
    queues[7] = creamsg(QREGISTROCAJOUTID, QREGISTROCAJOUTPATH);
    printf("Se generaron ipcs.\n");
}


bool iniciarTrabajadores(pid_t* cajero, pid_t* middleware, int* queues) {
    pid_t middleCajero = startCajeroMOM(queues);
    if (middleCajero == 0) {
        return false;
    }
    *middleware = middleCajero;
    printf("Inicio Middle cajero\n");
    pid_t cajeroHel = crearCajero();
    if (cajeroHel == 0) {
        return false;
    }
    cajero = &cajeroHel;
    printf("Se creo cajero.\n");
    return true;
}


pid_t simular() {
    pid_t simulador = fork();

    if(simulador == 0) {
        char input = '1';
        printf("Enviar caracter 'x' para terminar con la ejecucion\n");
        while(input != 'x') {
            input = getchar();
        }
        return 0;
    } else {
        return simulador;
    }
}

void cerrarIPCs(int* queues) {
    for(int q = 0; q < 8; q++) {
        if(q == 1 || q == 2 || q == 5) {
            close(queues[q]);
        }
        if(queues[q] != 0) {
            elimsg(queues[q]);
        }
    }
}