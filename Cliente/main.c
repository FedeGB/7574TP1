//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"
#include <inttypes.h>

int main(int argc, char** argv) {
    printf("Comienza cliente de heladeria\n");
    int queues[5];
    pid_t middleware;

    int controlador = start(queues, &middleware);
    if(controlador == 0) {
        return 0;
    }
    pid_t sim = simular(queues, &middleware);
    if (sim == 0) {
        return 0;
    }

    waitpid(sim, NULL, 0);

    cerrarIPCs(queues);
    return 0;
}