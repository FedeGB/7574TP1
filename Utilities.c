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
    int sleepingTime = numeroAleatorioRango(3, 7); // Entre 1 y 4 segundo
    sleep(sleepingTime);
}

int numeroAleatorioRango(int min, int max) {
    int random = rand();
    return ((random % (max - min + 1)) + min);
}

void getPedidoCierre(char* cierre) {
    char lastDelivery[4];
    lastDelivery[0] = '0';
    lastDelivery[1] = '1';
    lastDelivery[2] = '1';
    lastDelivery[3] = '1';
    strncpy(cierre, lastDelivery, 4);
}