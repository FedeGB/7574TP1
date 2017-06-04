//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"
#include <inttypes.h>

int main(int argc, char** argv) {

    printf("Comienza heladero en heladeria\n");
    int queues[6], semaforos[7];
    pid_t heladeros;
    pid_t middleware;

    int controlador = start(queues,  &heladeros, &middleware);
    if(controlador == 0) {
        return 0;
    }

    pid_t sim = simular();
    if(sim == 0)  {
        return 0;
    }
    waitpid(sim, NULL, 0);

    cerrarIPCs(queues);
    waitpid(heladeros, NULL, 0);

    return 0;
}