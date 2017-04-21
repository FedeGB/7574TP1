//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"

int main() {
    printf("Comienza heladeria\n");
    int queues[18], sharedMem[4], semaforos[12];
    pid_t cajero;
    pid_t heladeros[2];
    pid_t middlewares[3];

    int controlador = start(queues, sharedMem, semaforos, &cajero,  heladeros, middlewares);
    if(controlador == 0) {
        return 0;
    }
    pid_t sim = simular(queues, sharedMem, semaforos, &cajero,  heladeros, middlewares);
    if(sim == 0) {
        return 0;
    }
    waitpid(sim, NULL, 0);
    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}