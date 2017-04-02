//
// Created by fedenote on 3/19/17.
//

#include "Heladero.h"

pid_t crearHeladero() {
    pid_t proceso = fork();
    if(proceso == 0) {
        time_t t;
        srand((unsigned)time(&t));
        int queueHeladeros = getmsg(QHELADEROID, QHELADEROPATH);
        int queueRetirar = getmsg(QRETIRARID, QRETIRARPATH);
        bool flag = true;
        while(flag) {
            flag = atenderPedido(queueHeladeros, queueRetirar);
        }
        printf("Heladero %d: Termine de atender\n", getpid());
        return 0;
    } else {
        return proceso;
    }
}

bool atenderPedido(int queueHeladeros, int queueRetirar) {
    Message msgRcv;
    printf("Heladero %d: Espero pedido.\n", getpid());
    int status = recibirmsg(queueHeladeros, &msgRcv, sizeof(msgRcv), 0);
    if(status < 0) {
        return false;
    }
    if(msgRcv.data[0] == '0') {
        return false;
    }
    printf("Heladero %d: Recibi pedido.\n", getpid());
    prepararHelado(msgRcv.data);
    printf("Heladero %d: Devuelvo helado a cliente.\n", getpid());
    enviarmsg(queueRetirar, &msgRcv, sizeof(msgRcv));
    return true;
}

void prepararHelado(char* gustos) {
    int semGusto = 0;
    printf("Heladero %d: Preparo helado.\n", getpid());
    char gusto;
    for(int i = 0; i < 3; i++) {
        gusto = gustos[i];
        semGusto = getSemaforo(gusto, SEMGUSTOS);
        printf("Pongo %c\n", gusto);
        p(semGusto);
        esperarAleatorio();
        v(semGusto);
    }
}