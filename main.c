//
// Created by fedenote on 3/13/17.
//

#include "MemoriaCompartida.h"
#include <stdio.h>
#include <unistd.h>
#include "ColaMensajes.h"
#include "Semaforo.h"
#include "Constantes.h"
#include "Cajero.h"

int main() {
    printf("Comienza heladeria");
    int queueCajero = creamsg(QCAJEROID, QCAJEROPATH);
    int queueHeladoros = creamsg(QHELADEROID, QHELADEROPATH);
    int semCajero = crearSemaforo(SEMCAJEROPATH, SEMCAJEROID, 0);

    pid_t cajero = crearCajero(queueCajero, semCajero, queueHeladoros);


    return 0;
}