//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


int start(int* queues, int* sharedMem, int* semaforos, pid_t* middleware) {
    iniciarIPCs(queues, sharedMem, semaforos);
    iniciarSharedMemories(sharedMem, semaforos);
    bool trabajador = iniciarTrabajadores(middleware, queues);
    if(trabajador) {
        return 1;
    } else {
        return 0;
    }
}


void iniciarIPCs(int* queues, int* sharedMem, int* semaforos) {
    int listener = createSocket(CLIENTEIP, CLIENTEPORT, true);
    queues[0] = listener;
    queues[1] = listener;
    queues[2] = creamsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    queues[3] = creamsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    queues[4] = creamsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
    queues[5] = creamsg(QREGISTROCLINID, QREGISTROCLINPATH);
    queues[6] = creamsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
    sharedMem[0] = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    sharedMem[1] = creashm(LUGARESCAJEROID, sizeof(int), LUGARESCAJEROPATH);
    sharedMem[2] = creashm(ENTRADAID, sizeof(bool), ENTRADAPATH);
    semaforos[0] = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 2);
    semaforos[1] = crearSemaforo(SEMLUGARESCAJPATH, SEMLUGARESCAJID, 1);
    semaforos[2] = crearSemaforo(SEMENTRADAPATH, SEMENTRADAID, 1);
    printf("Se generaron ipcs.\n");
}

void iniciarSharedMemories(int* sharedMem, int* semaforos) {
    if (sharedMem[0] != 0) {
        p(semaforos[0]);
        int *lugaresH = (int *) map(sharedMem[0]);
        *lugaresH = ESPACIOHELADERIA;
        unmap(lugaresH);
        v(semaforos[0]);

    }
    if(sharedMem[1] != 0) {
        p(semaforos[1]);
        int *lugaresC = (int *) map(sharedMem[1]);
        *lugaresC = 0;
        unmap(lugaresC);
        v(semaforos[1]);
    }
    if(sharedMem[2] != 0) {
        p(semaforos[2]);
        bool* entrada = (bool*)map(sharedMem[2]);
        (*entrada) = true;
        unmap(entrada);
        v(semaforos[2]);
    }
    printf("Se incializaron variables de memoria compartida.\n");
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


pid_t simular(int* queues, int* sharedMem, int* semaforos, pid_t* middle) {
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

        printf("Se cierra heladeria, se espera a que terminen los clientes.\n");
        p(semaforos[10]);
        bool* entrada = (bool*)map(sharedMem[2]);
        *entrada = false;
        unmap(entrada);
        v(semaforos[10]);

        p(semaforos[0]);
        int* lugaresH = (int*)map(sharedMem[0]);
        *lugaresH = 0;
        unmap(lugaresH);
        v(semaforos[0]);
        p(semaforos[9]);
        int* lugaresC = (int*)map(sharedMem[1]);
        *lugaresC = MAXCOLACAJER + 100;
        unmap(lugaresC);
        v(semaforos[9]);
        for(std::vector<pid_t>::iterator it = pid_clientes.begin(); it != pid_clientes.end(); it++) {
            waitpid(*it, NULL, 0);
        }
        return 0;
    } else {
        return simulador;
    }
}

void cerrarIPCs(int* queues, int* sharedMem, int* semaforos) {
    for(int q = 0; q < 7; q++) {
        if(q == 0 || q == 1) {
            close(queues[q]);
        } else {
            elimsg(queues[q]);
        }
    }
    for(int sh = 0; sh < 3; sh++) {
        if(sharedMem[sh] != 0) {
            elishm(sharedMem[sh]);
        }
    }
    for(int sem = 0; sem < 3; sem++) {
        if(semaforos[sem] != 0) {
            eliminarSemaforo(semaforos[sem]);
        }
    }
}