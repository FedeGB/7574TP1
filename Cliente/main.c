//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"
#include <inttypes.h>

int main(int argc, char** argv) {
    printf("Comienza cliente de heladeria\n");
    int queues[6], sharedMem[3], semaforos[3];
    pid_t middleware;

    int controlador = start(queues, sharedMem, semaforos, &middleware);
    if(controlador == 0) {
        return 0;
    }
    pid_t sim = simular(queues, sharedMem, semaforos, &middleware);
    if (sim == 0) {
        return 0;
    }

    waitpid(sim, NULL, 0);

    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}