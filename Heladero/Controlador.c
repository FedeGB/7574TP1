//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


int start(int* queues, int* semaforos, pid_t* heladeros, pid_t* middleware) {
    iniciarIPCs(queues, semaforos);
    bool trabajador = iniciarTrabajadores(heladeros, middleware, queues);
    if(trabajador) {
        return 1;
    } else {
        return 0;
    }
}


void iniciarIPCs(int* queues, int* semaforos) {
        queues[0] = createSocket("", 0, false); // Para broker
        queues[1] = creamsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
        queues[2] = creamsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
//        queues[3] = createSocket("", 0, false);
        queues[3] = creamsg(QREGISTROHELINID, QREGISTROHELINPATH);
        queues[4] = creamsg(QREGISTROHELOUTID, QREGISTROHELOUTPATH);
        semaforos[0] = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
        semaforos[1] = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
        semaforos[2] = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
        semaforos[3] = crearSemaforo(SEMGUSTOS, LIMON, 1);
        semaforos[4] = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
        semaforos[5] = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
        semaforos[6] = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);
    printf("Se generaron ipcs.\n");
}

bool iniciarTrabajadores(pid_t* heladeros, pid_t* middleware, int* queues) {
    pid_t middleHeladeros = startHeladeroMOM(queues);
    if (middleHeladeros == 0) {
        return false;
    }
    *middleware = middleHeladeros;
    printf("Inicio Middle heladeros\n");
    for (int hel = 0; hel < 2; hel++) {
        pid_t helade = crearHeladero();
        if (helade == 0) {
            return false;
        }
        heladeros[hel] = helade;
    }
    printf("Se crearon 2 heladeros.\n");

    return true;
}


pid_t simular() {
    pid_t simulador = fork();

    if(simulador == 0) {
        char input = '1';
        printf("Enviar caracter x para terminar proceso.\n");
        while(input != 'x') {
            input = getchar();
        }
        return 0;
    } else {
        return simulador;
    }
}

void cerrarIPCs(int* queues, int* semaforos) {
    for(int q = 0; q < 6; q++) {
        if(q == 0 || q == 3) {
            close(queues[q]);
        } else {
            elimsg(queues[q]);
        }
    }
    for(int sem = 0; sem < 7; sem++) {
        eliminarSemaforo(semaforos[sem]);

    }
}