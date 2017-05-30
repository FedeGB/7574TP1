//
// Created by fedenote on 5/26/17.
//


#include "Controlador.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
    int queues[3];
    int sharedmem[2];
    int semaforo;
    pid_t listener;
    pid_t router;
    pid_t init = initBroker(queues, sharedmem, &semaforo, &listener, &router);
    if(init == 0) {
        return 0;
    }
    // TODO: Hago un getinput como en los otros y si aparece x cierro IPCS y dejo que muera todo?
    waitpid(init, NULL, 0);
    // TODO: Necesito esperar por estos dos ultimos realmente?
    waitpid(listener, NULL, 0);
    waitpid(router, NULL, 0);
    cerrarIPCs(queues, sharedmem, &semaforo);

    return 0;
}