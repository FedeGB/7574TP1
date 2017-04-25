//
// Created by fedenote on 3/18/17.
//

#include "Cajero.h"


pid_t crearCajero() {
    pid_t cajeroId = fork();

    if(cajeroId == 0) {
        time_t t;
        srand((unsigned)time(&t));
        // Uso real mas adelante
        long idCajero = registrarCajero();
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