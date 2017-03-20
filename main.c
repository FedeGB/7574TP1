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
    int queueHeladeros = creamsg(QHELADEROID, QHELADEROPATH);
    int queueRetirar = creamsg(QRETIRARID, QRETIRARPATH);
    int lugaresHeladeria = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    int semLugares = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 1);
    int semCajero = crearSemaforo(SEMCAJEROPATH, SEMCAJEROID, 0);
    int semVainilla = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
    int semFrutilla = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
    int semDulce = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
    int semLimon = crearSemaforo(SEMGUSTOS, LIMON, 1);
    int semSambayon = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
    int semCrema = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
    int semMenta = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);

    pid_t cajero = crearCajero();

//    waitpid(cajero);

    elimsg(queueCajero);
    elimsg(queueHeladeros);
    elimsg(queueRetirar);
    elishm(lugaresHeladeria);
    eliminarSemaforo(semLugares);
    eliminarSemaforo(semCajero);
    eliminarSemaforo(semVainilla);
    eliminarSemaforo(semFrutilla);
    eliminarSemaforo(semDulce);
    eliminarSemaforo(semLimon);
    eliminarSemaforo(semSambayon);
    eliminarSemaforo(semCrema);
    eliminarSemaforo(semMenta);

    return 0;
}