//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


pid_t start(int* queues, int* sharedMem, int* semaforos) {
    pid_t controlador = fork();

    if(controlador == 0) {
        return 0;
    } else {
        iniciarIPCs(queuesm, sharedMem, semaforos);
        iniciarSharedMemories(sharedMem, semaforos);
        pid_t cajero;
        pid_t heladeros[2];
        iniciarTrabajadores(&cajero, heladeros);
        return controlador;
    }
}


void iniciarIPCs(int* queues, int* sharedMem, int* semaforos) {
    int queueCajero = creamsg(QCAJEROID, QCAJEROPATH);
    int queueHeladeros = creamsg(QHELADEROID, QHELADEROPATH);
    int queueRetirar = creamsg(QRETIRARID, QRETIRARPATH);
    queues[0] = queueCajero;
    queues[1] = queueHeladeros;
    queues[2] = queueRetirar;
    int lugaresHeladeria = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    int lugaresCajero = creashm(LUGARESCAJEROID, sizeof(int), LUGARESCAJEROPATH);
    int entradaShm = creashm(ENTRADAID, sizeof(bool), ENTRADAPATH);
    sharedMem[0] = lugaresHeladeria;
    sharedMem[1] = lugaresCajero;
    sharedMem[2] = entradaShm;
    int semLugares = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 2);
    int semCajero = crearSemaforo(SEMCAJEROPATH, SEMCAJEROID, 0);
    int semVainilla = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
    int semFrutilla = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
    int semDulce = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
    int semLimon = crearSemaforo(SEMGUSTOS, LIMON, 1);
    int semSambayon = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
    int semCrema = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
    int semMenta = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);
    int semLugaresCaj = crearSemaforo(SEMLUGARESCAJPATH, SEMLUGARESCAJID, 1);
    int semEntrada = crearSemaforo(SEMENTRADAPATH, SEMENTRADAID, 1);
    semaforos[0] = semLugares;
    semaforos[1] = semCajero;
    semaforos[2] = semVainilla;
    semaforos[3] = semFrutilla;
    semaforos[4] = semDulce;
    semaforos[5] = semLimon;
    semaforos[6] = semSambayon;
    semaforos[7] = semCrema;
    semaforos[8] = semMenta;
    semaforos[9] = semLugaresCaj;
    semaforos[10] = semEntrada;
    printf("Se generaron ipcs.\n");
}

void iniciarSharedMemories(int* sharedMem, int* semaforos) {
    p(semaforos[0]);
    int* lugaresH = (int*)map(sharedMem[0]);
    *lugaresH = ESPACIOHELADERIA;
    unmap(lugaresH);
    v(semaforos[0]);
    p(semaforos[9]);
    int* lugaresC = (int*)map(sharedMem[1]);
    *lugaresC = 0;
    unmap(lugaresC);
    v(semaforos[9]);
    p(semaforos[10]);
    bool* entrada = (bool*)map(sharedMem[2]);
    (*entrada) = true;
    unmap(entrada);
    v(semaforos[10]);
    printf("Se incializaron variables de memoria compartida.\n");
}


void iniciarTrabajadores(pid_t cajero, pid_t* heladeros) {
    pid_t cajero;
    pid_t heladero1;
    pid_t heladero2;
    cajero = crearCajero();
    if(cajero == 0) {
        return;
    }
    printf("Se creo cajero.\n");

    for(int hel = 0; hel < 2; hel++) {
        heladeros[hel] = crearHeladero();
        if (heladeros[hel] == 0) {
            return;
        }
    }
    printf("Se crearon 2 heladeros.\n");
}

void cerrarIPCs(int* queues, int* sharedMem, int* semaforos) {
    for(int q = 0; q < 3; q++) {
        elimsg(queues[q]);
    }
    for(int sh = 0; sh < 3; sh++) {
        elishm(sharedMem[sh]);
    }
    for(int sem = 0; sem < 11; sem++) {
        eliminarSemaforo(semaforos[sem]);
    }
}