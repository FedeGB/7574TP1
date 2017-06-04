//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


int start(int* queues, pid_t* middleware) {
    iniciarIPCs(queues);
    bool trabajador = iniciarTrabajadores(middleware, queues);
    if(trabajador) {
        return 1;
    } else {
        return 0;
    }
}


void iniciarIPCs(int* queues) {
    int listener = createSocket("", 0, false);
    queues[0] = listener;
    queues[1] = creamsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    queues[2] = creamsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    queues[3] = creamsg(QREGISTROCLINID, QREGISTROCLINPATH);
    queues[4] = creamsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
    printf("Se generaron ipcs.\n");
}

bool iniciarTrabajadores(pid_t* middleware, int* queues) {
    pid_t middleClientes = startClienteMOM(queues);
    if (middleClientes == 0) {
        return false;
    }
    *middleware = middleClientes;
    printf("Inicio Middle clientes\n");

    return true;
}


pid_t simular(int* queues, pid_t* middle) {
    pid_t simulador = fork();

    if(simulador == 0) {
        std::vector<pid_t> pid_clientes;
        char input = '1';

        while(input != 'x') {
            input = getchar();
            if(input == 'c'){
                pid_t cliente = generarCliente();
                if(cliente == 0) {
                    return 0;
                } else if (cliente > 0) {
                    printf("Se creo cliente %d.\n", cliente);
                    pid_clientes.push_back(cliente);
                }
            }
        }
        return 0;
    } else {
        return simulador;
    }
}

void cerrarIPCs(int* queues) {
    for(int q = 0; q < 5; q++) {
        if(q == 0) {
            close(queues[q]);
        } else {
            elimsg(queues[q]);
        }
    }
}