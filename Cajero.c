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
    int i = 1;
    int j = 0;
    while(flag) {
        v(semQueueId);
        printf("Cajero: Atiendo cliente.\n");
        esperarAleatorio();
        Message msgRcv;
        int status = recibirmsg(queueCliente, &msgRcv, sizeof(msgRcv), 0);
        printf("Cajero: Atiendo pedido %s.\n", msgRcv.data);
        if(status >= 0) {
            if(msgRcv.data[0] == '0') {
                flag = false;
                i = 2;
            }
            while(j < i) {
                enviarmsg(queueHeladeros, &msgRcv, sizeof(msgRcv));
                j++;
            }
            printf("Cajero: Envie mensaje a heladeros.\n");
        }
        j = 0;
        i = 1;
    }
}
