//
// Created by fedenote on 3/18/17.
//

#include "Cajero.h"


pid_t crearCajero() {
    pid_t cajeroId = fork();

    if(cajeroId == 0) {
        time_t t;
        srand((unsigned)time(&t));
        atenderCajero();
        printf("Cajero: Termine de atender\n");
        return 0;
    } else {
        return cajeroId;
    }
}

void atenderCajero() {
    int semQueueId = getSemaforo(SEMCAJEROID, SEMCAJEROPATH);
    while(true) {
        v(semQueueId);
        printf("Cajero: Atiendo cliente.\n");
        esperarAleatorio();
        char pedido[4];
        long idCliente;
        if(!recibirPedidoCliente(pedido, &idCliente)) {
            break;
        }
        printf("Cajero: Atiendo pedido %s.\n", pedido);
        Message msgRcv;
        enviarPedidoHeladero(pedido, idCliente);
        printf("Cajero: Envie mensaje a heladeros.\n");
    }
}


//bool hayClientesEnCola() {
//    int semCajero = getSemaforo(SEMLUGARESID, SEMLUGARESPATH);
//    int shQCajero = getshm(LUGARESCAJEROID, LUGARESCAJEROPATH);
//    bool hay = true;
//    p(semCajero);
//    int* enCola = (int*)map(shQCajero);
//    if(*enCola <= 0) {
//        hay = false;
//    }
//    unmap(enCola);
//    v(semCajero);
//    return hay;
//}