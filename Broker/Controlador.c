//
// Created by fedenote on 5/26/17.
//

#include "Controlador.h"


pid_t initBroker(int* queues, int* sharedmem, int* semaforo, pid_t* listener, pid_t* router) {
    pid_t initializer = fork();
    if(initializer == 0) {
        inicializarIPCS(queues, sharedmem, semaforo);
        bool workers = inicializarTrabajadores(queues, listener, router);
        if(workers) {
            return 1;
        }
        return 0;
    } else {
        return initializer;
    }
}


void inicializarIPCS(int* queues, int* sharedmem, int* semaforo) {
    queues[0] = creamsg(QINCOMINGID, QINCOMINGPATH);
    queues[1] = creamsg(QOUTGOINGID, QOUTGOINGPATH);
    queues[2] = createSocket(BROKERIP, BROKERPORT, true);
    sharedmem[0] = creashm(SHAREDTABLEID, sizeof(entity) * 1000, SHAREDTABLEPATH);
    sharedmem[1] = creashm(SHAREDCANTID, sizeof(int), SHAREDCANTPATH);
    *semaforo = crearSemaforo(SEMTABLEPATH, SEMTABLEID, 1);
    p(*semaforo);
    int* cantidad = (int*)map(sharedmem[1]);
    *cantidad = 0;
    unmap(cantidad);
    v(*semaforo);
    printf("Se iniciaron IPCS.\n");
}

bool inicializarTrabajadores(int* queues, pid_t* listener, pid_t* router) {
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
    return true;
}


void cerrarIPCs(int* queues, int* sharedmem, int* semaforo) {
    elimsg(queues[0]);
    elimsg(queues[1]);
    close(queues[2]);
    eliminarSemaforo(*semaforo);
    elishm(sharedmem[0]);
    elishm(sharedmem[1]);
}