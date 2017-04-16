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
    bool flag = true;
    int i = 1;
    int j = 0;
    while(flag) {
        v(semQueueId);
        printf("Cajero: Atiendo cliente.\n");
        esperarAleatorio();
        char pedido[4];
        long idCliente;
        recibirPedidoCliente(pedido, &idCliente);
        printf("Cajero: Atiendo pedido %s.\n", pedido);
        // TODO: Generar ticket y mandar a cliente
        if(pedido[0] == '0') {
            flag = false;
            i = 2;
        }
        Message msgRcv;
        while(j < i) {
            enviarPedidoHeladero(pedido, idCliente);
            j++;
        }
        printf("Cajero: Envie mensaje a heladeros.\n");
        j = 0;
        i = 1;
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