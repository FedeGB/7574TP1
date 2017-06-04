//
// Created by fedenote on 5/26/17.
//

#include "Controlador.h"


pid_t initBroker(int* queues, int* sharedmem, int* semaforos, pid_t* listener, pid_t* router, pid_t* adminPotes, pid_t* adminHel) {
    pid_t initializer = fork();
    if(initializer == 0) {
        inicializarIPCS(queues, sharedmem, semaforos);
        bool workers = inicializarTrabajadores(queues, listener, router, adminPotes, adminHel);
        if(workers) {
            return 1;
        }
        return 0;
    } else {
        return initializer;
    }
}


void inicializarIPCS(int* queues, int* sharedmem, int* semaforos) {
    queues[0] = creamsg(QINCOMINGID, QINCOMINGPATH);
    queues[1] = creamsg(QOUTGOINGID, QOUTGOINGPATH);
    queues[2] = createSocket(BROKERIP, BROKERPORT, true);
    queues[3] = creamsg(QINGUSTOSID, QINGUSTOSPATH);
    queues[4] = creamsg(QOUTGUSTOSID, QOUTGUSTOSPATH);
    queues[5] = creamsg(QINHELADERIAID, QINHELADERIAPATH);
    queues[6] = creamsg(QOUTHELADERIAID, QOUTHELADERIAPATH);
    sharedmem[0] = creashm(SHAREDTABLEID, sizeof(entity) * 1000, SHAREDTABLEPATH);
    sharedmem[1] = creashm(SHAREDCANTID, sizeof(int), SHAREDCANTPATH);
    sharedmem[2] = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    sharedmem[3] = creashm(LUGARESCAJEROID, sizeof(int), LUGARESCAJEROPATH);
    sharedmem[4] = creashm(ENTRADAID, sizeof(bool), ENTRADAPATH);
    semaforos[0] = crearSemaforo(SEMTABLEPATH, SEMTABLEID, 1);
    semaforos[1] = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
    semaforos[2] = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
    semaforos[3] = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
    semaforos[4] = crearSemaforo(SEMGUSTOS, LIMON, 1);
    semaforos[5] = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
    semaforos[6] = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
    semaforos[7] = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);
    semaforos[8] = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 2);
    semaforos[9] = crearSemaforo(SEMLUGARESCAJPATH, SEMLUGARESCAJID, 1);
    semaforos[10] = crearSemaforo(SEMENTRADAPATH, SEMENTRADAID, 1);

    p(semaforos[0]);
    int* cantidad = (int*)map(sharedmem[1]);
    *cantidad = 0;
    unmap(cantidad);
    v(semaforos[0]);

    p(semaforos[8]);
    int *lugaresH = (int *) map(sharedmem[2]);
    *lugaresH = ESPACIOHELADERIA;
    unmap(lugaresH);
    v(semaforos[8]);

    p(semaforos[9]);
    int *lugaresC = (int *) map(sharedmem[3]);
    *lugaresC = 0;
    unmap(lugaresC);
    v(semaforos[9]);

    p(semaforos[10]);
    bool* entrada = (bool*)map(sharedmem[4]);
    (*entrada) = true;
    unmap(entrada);
    v(semaforos[10]);

    printf("Se iniciaron IPCS.\n");
}

bool inicializarTrabajadores(int* queues, pid_t* listener, pid_t* router, pid_t* potAdmin, pid_t* helAdmin) {
    pid_t list = startListener(queues[2]);
    if(list == 0) {
        return false;
    }
    *listener = list;
    printf("Se inicio listener.\n");
    pid_t route = startRouter(queues[0], queues[1]);
    if(route == 0) {
        return false;
    }
    *router = route;
    printf("Se inicio router.\n");
    pid_t gustos = startPoteAdmin(queues[3], queues[4]);
    if(gustos == 0) {
        return false;
    }
    *potAdmin = gustos;
    pid_t heladeria = startHeladeriaAdmin(queues[5], queues[6]);
    if(heladeria == 0) {
        return false;
    }
    *helAdmin = heladeria;
    printf("Se inicio administrador de potes de gustos\n");
    return true;
}


void cerrarIPCs(int* queues, int* sharedmem, int* semaforos) {
    elimsg(queues[0]);
    elimsg(queues[1]);
    close(queues[2]);
    elimsg(queues[3]);
    elimsg(queues[4]);
    elimsg(queues[5]);
    elimsg(queues[6]);
    for(int sem = 0; sem < 11; sem++) {
        eliminarSemaforo(semaforos[sem]);
    }
    for(int sh = 0; sh < 5; sh++) {
        if(sharedmem[sh] != 0) {
            elishm(sharedmem[sh]);
        }
    }
}