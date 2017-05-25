//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"
#include <inttypes.h>

int main(int argc, char** argv) {

    printf("Comienza heladero en heladeria\n");
    int queues[6], semaforos[7];
    pid_t heladeros[2];
    pid_t middleware;

    int controlador = start(queues, semaforos,  heladeros, &middleware);
    if(controlador == 0) {
        return 0;
    }

    pid_t sim = simular();
    if(sim == 0)  {
        return 0;
    }
    waitpid(sim, NULL, 0);

    cerrarIPCs(queues, semaforos);

    for(int hel; hel < 2; hel++) {
        waitpid(heladeros[hel], NULL, 0);
    }
    return 0;
}