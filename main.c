//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"
#include <inttypes.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Se necesita indicar que de las 3 partes iniciar:\n./main <numero>\nDonde numero 1: Cliente, 2: Cajero, 3: Heladero\n");
        return 0;
    }
    uintmax_t num = strtoumax(argv[1], NULL, 10);
    if (num == UINTMAX_MAX && errno == ERANGE) {
        printf("El parametro que indica que iniciar debe ser un numero:\n1: Cliente, 2: Cajero, 3: Heladero\n");
        return 0;
    }
    int init = num;
    printf("Comienza heladeria\n");
    int queues[18], sharedMem[4], semaforos[12];
    pid_t cajero;
    pid_t heladeros[2];
    pid_t middlewares[3];

    int controlador = start(queues, sharedMem, semaforos, &cajero,  heladeros, middlewares, init);
    if(controlador == 0) {
        return 0;
    }
    if(init == 1) {
        pid_t sim = simular(queues, sharedMem, semaforos, &cajero, heladeros, middlewares);
        if (sim == 0) {
            return 0;
        }
        waitpid(sim, NULL, 0);
    } else if(init == 2) {
        waitpid(cajero, NULL, 0);
        while(1) {
            sleep(1);
        }
    } else {
        for(int hel; hel < 2; hel++) {
            waitpid(heladeros[hel], NULL, 0);
        }
        while(1) {
            sleep(1);
        }
    }
    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}