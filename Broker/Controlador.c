//
// Created by fedenote on 5/26/17.
//

#include "Controlador.h"


pid_t initBroker(int* queues, pid_t* listener, pid_t* router) {
    pid_t initializer = fork();
    if(initializer == 0) {
        inicializarIPCS(queues);
        bool workers = inicializarTrabajadores(queues, listener, router);
        if(workers) {
            return 1;
        }
        return 0;
    } else {
        return initializer;
    }
}


void inicializarIPCS(int* queues) {
    queues[0] = creamsg(QINCOMINGID, QINCOMINGPATH);
    queues[1] = creamsg(QOUTGOINGID, QOUTGOINGPATH);
    queues[2] = createSocket(BROKERIP, BROKERPORT, true);
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


void cerrarIPCs(int* queues) {
    elimsg(queues[0]);
    elimsg(queues[1]);
    close(queues[2]);
}