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
            atenderPedido(queueHeladeros, queueRetirar);
        }
//        return  0;
    } else {
        return proceso;
    }
}

void atenderPedido(int queueHeladeros, int queueRetirar) {
    Message msgRcv;
    recibirmsg(queueHeladeros, &msgRcv, sizeof(msgRcv), 0);
    prepararHelado(msgRcv.data);
    enviarmsg(queueRetirar, &msgRcv, sizeof(msgRcv));
}

void prepararHelado(char* gustos) {
    int semVainilla = getSemaforo(VAINILLA, SEMGUSTOS);
    int semFrutilla = getSemaforo(FRUTILLA, SEMGUSTOS);
    int semDulce = getSemaforo(DULCEDELECHE, SEMGUSTOS);
    int semLimon = getSemaforo(LIMON, SEMGUSTOS);
    int semSambayon = getSemaforo(SAMBAYON, SEMGUSTOS);
    int semCrema = getSemaforo(CREMAAMERICANA, SEMGUSTOS);
    int semMenta = getSemaforo(MENTAGRANIZADA, SEMGUSTOS);

    for(int i = 0; i < 3; i++) {
        char gusto = gustos[i];
        switch(gusto){
            case VAINILLA:
                p(semVainilla);
                sleep(3);
                v(semVainilla);
                break;
            case FRUTILLA:
                p(semFrutilla);
                sleep(3);
                v(semFrutilla);
                break;
            case DULCEDELECHE:
                p(semDulce);
                sleep(3);
                v(semDulce);
                break;
            case LIMON:
                p(semLimon);
                sleep(3);
                v(semLimon);
                break;
            case SAMBAYON:
                p(semSambayon);
                sleep(3);
                v(semSambayon);
                break;
            case CREMAAMERICANA:
                p(semCrema);
                sleep(3);
                v(semCrema);
                break;
            case MENTAGRANIZADA:
                p(semMenta);
                sleep(3);
                v(semMenta);
                break;
        }
    }
}