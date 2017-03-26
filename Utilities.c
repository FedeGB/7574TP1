//
// Created by fedenote on 3/24/17.
//

#include "Utilities.h"

bool seguirAtendiendo() {
    int semEntrada = getSemaforo(SEMENTRADAID, SEMENTRADAPATH);
    int entradaShm = getshm(ENTRADAID, ENTRADAPATH);
    p(semEntrada);
    bool* entrada = (bool*)map(entradaShm);
    bool flag = *entrada;
    unmap(entrada);
    v(semEntrada);
    return flag;
}


void esperarAleatorio() {
    time_t t;
    srand((unsigned)time(&t));
    int random = rand();
    int sleepingTime = (random % 5) + 1; // Entre 1 y 4 segundo
    sleep(sleepingTime);
}


void getPedidoCierre(char* cierre) {
    char lastDelivery[4];
    lastDelivery[0] = '0';
    lastDelivery[1] = '0';
    lastDelivery[2] = '0';
    lastDelivery[3] = '0';
    strncpy(cierre, lastDelivery, 4);
}