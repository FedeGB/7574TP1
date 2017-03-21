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
        printf("Cajero: Atiendo cliente.\n");
        sleep(2);
        Message msgRcv;
        int status = recibirmsg(queueCliente, &msgRcv, sizeof(msgRcv), 0);
        printf("Cajero: Atiendo pedido %s de %l.\n", msgRcv.data, msgRcv.mtype);
        if(status >= 0) {
            enviarmsg(queueHeladeros, &msgRcv, sizeof(msgRcv));
            printf("Cajero: Envie mensaje a heladeros.\n");
            int lugaresCajero = getshm(LUGARESCAJEROID, LUGARESCAJEROPATH);
            if(lugaresCajero > 0) {
                int semLugaresCaj = getSemaforo(SEMLUGARESCAJID, SEMLUGARESCAJPATH);
                p(semLugaresCaj);
                int *queue = (int *) map(lugaresCajero);
                (*queue)++;
                unmap(queue);
                v(semLugaresCaj);
            }
        }
    }
}
