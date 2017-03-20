//
// Created by fedenote on 3/12/17.
//

#include "Cliente.h"

pid_t generarCliente() {
    pid_t pid = fork();

    int queueHeladeros = getmsg(QHELADEROID, QHELADEROPATH);

    if(pid == 0) {
        if(todoOcupado()) {
            return 0;
        }
        realizarPedido();
        retirarPedido();
        return 0;
    } else {
        return pid;
    }
}

void realizarPedido() {
    int semQueueId = getSemaforo(SEMCAJEROID, SEMCAJEROPATH);
    int queueCliente = getmsg(QCAJEROID, QCAJEROPATH);
    sleep(3);
    p(semQueueId);
    char* pedido = getPedido();
    Message msgSnd;
    msgSnd.mtype = getpid();
    strncpy(msgSnd.data, pedido, 4);
    if(pedido) {
        free(pedido);
    }
    enviarmsg(queueCliente, &msgSnd, sizeof(msgSnd));
}

void retirarPedido() {
    int queueRetirar = getmsg(QRETIRARID, QRETIRARPATH);
    Message msgRcv;
    int status = recibirmsg(queueRetirar, &msgRcv, sizeof(msgRcv), getpid());
    if(status > 0) {
        if(msgRcv.data[3] == LLEVAR) {
            return;
        }
        sleep(3);
        int lugaresMem = getshm(LUGARESID, LUGARESPATH);
        int lugaresSem = getSemaforo(SEMLUGARESID, SEMLUGARESPATH);
        p(lugaresSem);
        int* lugares = (int*)map(lugaresMem);
        (*lugares)++;
        unmap(lugares);
        v(lugaresMem);
    }
}

// TODO
char* getPedido() {
    char* pedido = (char*)malloc(sizeof(char)*4);
    pedido[0] = DULCEDELECHE;
    pedido[1] = VAINILLA;
    pedido[2]= FRUTILLA;
    pedido[3] = ENLUGAR;
    return pedido;
}


bool todoOcupado() {
    int lugaresHeladeria = getshm(LUGARESID,LUGARESPATH);
    bool ocupado = false;
    if(lugaresHeladeria > 0) {
        int semLugares = getSemaforo(SEMLUGARESID, SEMLUGARESPATH);
        p(semLugares);
        int* lugares = (int*)map(lugaresHeladeria);
        if(*lugares <= 0) {
            ocupado = true;
        } else {
            (*lugares)--;
        }
        unmap(lugares);
        v(semLugares);
    }
    return ocupado;
}