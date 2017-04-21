//
// Created by fedenote on 3/24/17.
//

#include "Utilities.h"

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
    lastDelivery[1] = '0';
    lastDelivery[2] = '0';
    lastDelivery[3] = '0';
    strncpy(cierre, lastDelivery, 4);
}