//
// Created by fedenote on 3/18/17.
//

#include "Cajero.h"


pid_t crearCajero() {
    pid_t cajeroId = fork();

    if(cajeroId == 0) {
        atenderCajero();
        return 0;
    } else {
        return cajeroId;
    }
}


void atenderCajero() {
    int semQueueId = getSemaforo(SEMCAJEROID, SEMCAJEROPATH);
    int queueCliente = getmsg(QCAJEROID, QCAJEROPATH);
    int queueHeladeros = getmsg(QHELADEROID, QHELADEROPATH);

    bool flag = true;
    while(flag) {
        v(semQueueId);
//        esperarAleatorio();
        sleep(2);
        Message msgRcv;
        int status = recibirmsg(queueCliente, &msgRcv, sizeof(msgRcv), 0);
        if(status >= 0) {
            enviarmsg(queueHeladeros, &msgRcv, sizeof(msgRcv));
        }
    }
}
