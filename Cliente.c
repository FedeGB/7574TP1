//
// Created by fedenote on 3/12/17.
//

#include "Cliente.h"

pid_t generarCliente() {
    pid_t pid = fork();
    if(pid == 0) {
        if(todoOcupado()) {
            return 0;
        }
        time_t t;
        srand((unsigned)time(&t));
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
    p(semQueueId);
    char pedido[4];
    getPedido(pedido);
    Message msgSnd;
    msgSnd.mtype = getpid();
    strncpy(msgSnd.data, pedido, 4);
    printf("Cliente %d: envio pedido.\n", getpid());
    enviarmsg(queueCliente, &msgSnd, sizeof(msgSnd));
    int semLugaresCaj = getSemaforo(SEMLUGARESCAJID, SEMLUGARESCAJPATH);
    int lugaresCajero = getshm(LUGARESCAJEROID, LUGARESCAJEROPATH);
    p(semLugaresCaj);
    int* queue = (int*)map(lugaresCajero);
    (*queue)--;
    printf("Cliente %d: Libero lugar en cola.\n", getpid());
    unmap(queue);
    v(semLugaresCaj);
}

void retirarPedido() {
    int queueRetirar = getmsg(QRETIRARID, QRETIRARPATH);
    Message msgRcv;
    printf("Cliente %d: Estoy esperando mi pedido.\n", getpid());
    int status = recibirmsg(queueRetirar, &msgRcv, sizeof(msgRcv), getpid());
    printf("Cliente %d: Recibi mi pedido.\n", getpid());
    if(status > 0) {
        if(msgRcv.data[3] == LLEVAR) {
            printf("Cliente %d: Es para llevar, me fui.\n", getpid());
            return;
        }
        printf("Cliente %d: Como el helado adentro.\n", getpid());
        esperarAleatorio();
        int lugaresMem = getshm(LUGARESID, LUGARESPATH);
        int lugaresSem = getSemaforo(SEMLUGARESID, SEMLUGARESPATH);
        printf("Cliente %d: Libero lugar y me voy.\n", getpid());
        p(lugaresSem);
        int* lugares = (int*)map(lugaresMem);
        (*lugares)++;
        unmap(lugares);
        v(lugaresSem);
    }
}

void getPedido(char* pedido) {
    char pedidoNuevo[4];
    pedidoNuevo[0] = numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[1] = numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[2]= numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[3] = numeroAleatorioRango(1, 2) + '0';

    strncpy(pedido, pedidoNuevo, 4);
}


bool todoOcupado() {
    int semEntrada = getSemaforo(SEMENTRADAID, SEMENTRADAPATH);
    int entradaShm = getshm(ENTRADAID,ENTRADAPATH);
    p(semEntrada);
    bool* entrada = (bool*)map(entradaShm);
    if(!*entrada) {
        unmap(entrada);
        v(semEntrada);
        printf("Cliente %d: Heladeria cerrada, me voy\n", getpid());
        return true;
    }
    unmap(entrada);
    v(semEntrada);

    bool ocupadoHeladeria = false;
    int lugaresCajero = getshm(LUGARESCAJEROID, LUGARESCAJEROPATH);
    if(lugaresCajero > 0) {
        int semLugaresCaj = getSemaforo(SEMLUGARESCAJID, SEMLUGARESCAJPATH);
        p(semLugaresCaj);
        int* queue = (int*)map(lugaresCajero);
        if((*queue) >= MAXCOLACAJER) {
            printf("Cliente %d: Cola esta llena, me voy.\n", getpid());
            unmap(queue);
            v(semLugaresCaj);
            return true;
        }
        (*queue)++;
        unmap(queue);
        v(semLugaresCaj);
        int lugaresHeladeria = getshm(LUGARESID,LUGARESPATH);
        int semLugares = getSemaforo(SEMLUGARESID, SEMLUGARESPATH);
        p(semLugares);
        int* lugares = (int*)map(lugaresHeladeria);
        if(*lugares <= 0) {
            printf("Cliente %d: Esta todo ocupado me voy.\n", getpid());
            ocupadoHeladeria = true;
        } else {
            (*lugares)--;
        }
        unmap(lugares);
        v(semLugares);
    }
    return ocupadoHeladeria;
}