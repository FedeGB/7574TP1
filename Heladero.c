//
// Created by fedenote on 3/19/17.
//

#include "Heladero.h"

pid_t crearHeladero() {
    pid_t proceso = fork();
    if(proceso == 0) {
        int queueHeladeros = getmsg(QHELADEROID, QHELADEROPATH);
        int queueRetirar = getmsg(QRETIRARID, QRETIRARPATH);
        bool flag = true;
        while(flag) {
            flag = atenderPedido(queueHeladeros, queueRetirar);
        }
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
    int semVainilla = getSemaforo(VAINILLA, SEMGUSTOS);
    int semFrutilla = getSemaforo(FRUTILLA, SEMGUSTOS);
    int semDulce = getSemaforo(DULCEDELECHE, SEMGUSTOS);
    int semLimon = getSemaforo(LIMON, SEMGUSTOS);
    int semSambayon = getSemaforo(SAMBAYON, SEMGUSTOS);
    int semCrema = getSemaforo(CREMAAMERICANA, SEMGUSTOS);
    int semMenta = getSemaforo(MENTAGRANIZADA, SEMGUSTOS);
    printf("Heladero %d: Preparo helado.\n", getpid());
    for(int i = 0; i < 3; i++) {
        char gusto = gustos[i];
        switch(gusto){
            case VAINILLA:
                p(semVainilla);
                printf("Heladero %d: Pongo vainilla.\n", getpid());
                sleep(3);
                v(semVainilla);
                break;
            case FRUTILLA:
                p(semFrutilla);
                printf("Heladero %d: Pongo frutilla.\n", getpid());
                sleep(3);
                v(semFrutilla);
                break;
            case DULCEDELECHE:
                p(semDulce);
                printf("Heladero %d: Pongo dulce de leche.\n", getpid());
                sleep(3);
                v(semDulce);
                break;
            case LIMON:
                p(semLimon);
                printf("Heladero %d: Pongo limon.\n", getpid());
                sleep(3);
                v(semLimon);
                break;
            case SAMBAYON:
                p(semSambayon);
                printf("Heladero %d: Pongo sambayon.\n", getpid());
                sleep(3);
                v(semSambayon);
                break;
            case CREMAAMERICANA:
                p(semCrema);
                printf("Heladero %d: Pongo crema.\n", getpid());
                sleep(3);
                v(semCrema);
                break;
            case MENTAGRANIZADA:
                p(semMenta);
                printf("Heladero %d: Pongo menta.\n", getpid());
                sleep(3);
                v(semMenta);
                break;
        }
    }
}