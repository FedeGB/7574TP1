//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"

int main() {
    printf("Comienza heladeria\n");
    int queues[18], sharedMem[3], semaforos[11];
    pid_t cajero;
    pid_t heladeros[2];

    int controlador = start(queues, sharedMem, semaforos, &cajero,  heladeros);
    if(controlador == 0) {
        return 0;
    }
    pid_t sim = simular(queues, sharedMem, semaforos, &cajero,  heladeros);
    if(sim == 0) {
        return 0;
    }
    waitpid(sim, NULL, 0);

    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}