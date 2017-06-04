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
        char hand[5];
        strncpy(hand, "j0000", 5);
        realizarHandShake(hand, idCajero);
        atenderCajero();
        printf("Cajero: Termine de atender\n");
        return 0;
    } else {
        return cajeroId;
    }
}

void atenderCajero() {
    while(true) {
        printf("Cajero: Atiendo cliente.\n");
        esperarAleatorio();
        char pedido[5];
        long idCliente;
        if(!recibirPedidoCliente(pedido, &idCliente)) {
            break;
        }
        char ticket[5];
        strncpy(ticket, "ticke", 5);
        if(!enviarTicketCliente(ticket, idCliente)) {
            break;
        }
        printf("Cajero: Atiendo pedido %s.\n", pedido);
        Message msgRcv;
        enviarPedidoHeladero(pedido, idCliente);
        printf("Cajero: Envie mensaje a heladeros.\n");
    }
}