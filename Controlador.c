//
// Created by fedenote on 3/28/17.
//

#include "Controlador.h"


int start(int* queues, int* sharedMem, int* semaforos, pid_t* cajero, pid_t* heladeros, pid_t* middlewares) {
    iniciarIPCs(queues, sharedMem, semaforos);
    iniciarSharedMemories(sharedMem, semaforos);
    bool trabajador = iniciarTrabajadores(cajero, heladeros, middlewares);
    if(trabajador) {
        return 1;
    } else {
        return 0;
    }
}


void iniciarIPCs(int* queues, int* sharedMem, int* semaforos) {
    queues[0] = creamsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
    queues[1] = creamsg(QCAJEROTOCLID, QCAJEROTOCLPATH);
    queues[2] = creamsg(QCLIENTETOCJID, QCLIENTETOCJPATH);
    queues[3] = creamsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
    queues[4] = creamsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
    queues[5] = creamsg(QCAJEROTOHELID, QCAJEROTOHELPATH);
    queues[6] = creamsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
    queues[7] = creamsg(QREGISTROCAJOUTID, QREGISTROCAJOUTPATH);
    queues[8] = creamsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
    queues[9] = creamsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    queues[10] = creamsg(QHELADEROTOCLID, QHELADEROTOCLPATH);
    queues[11] = creamsg(QREGISTROHELINID, QREGISTROHELINPATH);
    queues[12] = creamsg(QREGISTROHELOUTID, QREGISTROHELOUTPATH);
    queues[13] = creamsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    queues[14] = creamsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    queues[15] = creamsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
    queues[16] = creamsg(QREGISTROCLINID, QREGISTROCLINPATH);
    queues[17] = creamsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
    int lugaresHeladeria = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    int lugaresCajero = creashm(LUGARESCAJEROID, sizeof(int), LUGARESCAJEROPATH);
    int entradaShm = creashm(ENTRADAID, sizeof(bool), ENTRADAPATH);
    sharedMem[0] = lugaresHeladeria;
    sharedMem[1] = lugaresCajero;
    sharedMem[2] = entradaShm;
    int semLugares = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 2);
    int semCajero = crearSemaforo(SEMCAJEROPATH, SEMCAJEROID, 0);
    int semVainilla = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
    int semFrutilla = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
    int semDulce = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
    int semLimon = crearSemaforo(SEMGUSTOS, LIMON, 1);
    int semSambayon = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
    int semCrema = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
    int semMenta = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);
    int semLugaresCaj = crearSemaforo(SEMLUGARESCAJPATH, SEMLUGARESCAJID, 1);
    int semEntrada = crearSemaforo(SEMENTRADAPATH, SEMENTRADAID, 1);
    semaforos[0] = semLugares;
    semaforos[1] = semCajero;
    semaforos[2] = semVainilla;
    semaforos[3] = semFrutilla;
    semaforos[4] = semDulce;
    semaforos[5] = semLimon;
    semaforos[6] = semSambayon;
    semaforos[7] = semCrema;
    semaforos[8] = semMenta;
    semaforos[9] = semLugaresCaj;
    semaforos[10] = semEntrada;
    printf("Se generaron ipcs.\n");
}

void iniciarSharedMemories(int* sharedMem, int* semaforos) {
    p(semaforos[0]);
    int* lugaresH = (int*)map(sharedMem[0]);
    *lugaresH = ESPACIOHELADERIA;
    unmap(lugaresH);
    v(semaforos[0]);
    p(semaforos[9]);
    int* lugaresC = (int*)map(sharedMem[1]);
    *lugaresC = 0;
    unmap(lugaresC);
    v(semaforos[9]);
    p(semaforos[10]);
    bool* entrada = (bool*)map(sharedMem[2]);
    (*entrada) = true;
    unmap(entrada);
    v(semaforos[10]);
    printf("Se incializaron variables de memoria compartida.\n");
}


bool iniciarTrabajadores(pid_t* cajero, pid_t* heladeros, pid_t* middlewares) {
    pid_t middleCajero = startCajeroMOM();
    if(middleCajero == 0) {
        return false;
    }
    middlewares[0] = middleCajero;
    pid_t cajeroHel = crearCajero();
    if(cajeroHel == 0) {
        return false;
    }
    cajero = &cajeroHel;
    printf("Se creo cajero.\n");
    pid_t middleHeladeros = startHeladeroMOM();
    if(middleHeladeros == 0) {
        return false;
    }
    middlewares[1] = middleHeladeros;
    for(int hel = 0; hel < 2; hel++) {
        pid_t helade = crearHeladero();
        if (helade == 0) {
            return false;
        }
        heladeros[hel] = helade;
    }
    printf("Se crearon 2 heladeros.\n");
    pid_t middleClientes = startClienteMOM();
    if(middleClientes == 0) {
        return false;
    }
    middlewares[2] = middleClientes;
    return true;
}


pid_t simular(int* queues, int* sharedMem, int* semaforos, pid_t* cajero, pid_t* heladeros, pid_t* middle) {
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

        char cierre[4];
        Message msg;
        getPedidoCierre(cierre);
        msg.mtype = getpid();
        strncpy(msg.data, cierre, 4);

        p(semaforos[1]);
        enviarmsg(queues[0],&msg, sizeof(msg));
        waitpid(*cajero, NULL, 0);
        for(int hel = 0; hel < 2; hel++) {
            waitpid(heladeros[hel], NULL, 0);
        }
        for(std::vector<pid_t>::iterator it = pid_clientes.begin(); it != pid_clientes.end(); it++) {
            waitpid(*it, NULL, 0);
        }
        terminarMiddlewares();
        waitpid(middle[0], NULL, 0);
        waitpid(middle[1], NULL, 0);
        waitpid(middle[2], NULL, 0);
        return 0;
    } else {
        return simulador;
    }
}

void terminarMiddlewares() {
    Message msgClose;
    msgClose.data[0] = '1';
    msgClose.data[1] = '0';
    msgClose.data[2] = '1';
    msgClose.data[3] = '1';
    msgClose.mtype = 1;
    int queue1 = getmsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
    enviarmsg(queue1, &msgClose, sizeof(msgClose));
    int queue2 = getmsg(QCLIENTETOCJID, QCLIENTETOCJPATH);
    enviarmsg(queue2, &msgClose, sizeof(msgClose));
    int queue3 = getmsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
    enviarmsg(queue3, &msgClose, sizeof(msgClose));
    int queue4 = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    enviarmsg(queue4, &msgClose, sizeof(msgClose));
    int queue5 = getmsg(QCAJEROTOCLID, QCAJEROTOCLPATH);
    enviarmsg(queue5, &msgClose, sizeof(msgClose));
    int queue6 = getmsg(QHELADEROTOCLID, QHELADEROTOCLPATH);
    enviarmsg(queue6, &msgClose, sizeof(msgClose));
    int queue7 = getmsg(QCAJEROTOHELID, QCAJEROTOHELPATH);
    enviarmsg(queue7, &msgClose, sizeof(msgClose));
    int queue8 = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    enviarmsg(queue8, &msgClose, sizeof(msgClose));
    int registro1 = getmsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
    enviarmsg(registro1, &msgClose, sizeof(msgClose));
    int registro2 = getmsg(QREGISTROCLINID, QREGISTROCLINPATH);
    enviarmsg(registro2, &msgClose, sizeof(msgClose));
    int registro3 = getmsg(QREGISTROHELINID, QREGISTROHELINPATH);
    enviarmsg(registro3, &msgClose, sizeof(msgClose));
}

void cerrarIPCs(int* queues, int* sharedMem, int* semaforos) {
    // TODO: Modificar cantidades a cerrar
    for(int q = 0; q < 18; q++) {
        elimsg(queues[q]);
    }
    for(int sh = 0; sh < 3; sh++) {
        elishm(sharedMem[sh]);
    }
    for(int sem = 0; sem < 11; sem++) {
        eliminarSemaforo(semaforos[sem]);
    }
}