//
// Created by fedenote on 3/12/17.
//

#include "Cliente.h"

pid_t generarCliente() {
    pid_t pid = fork();
    if(pid == 0) {
        time_t t;
        srand((unsigned)time(&t));
        long idCliente = registrarCliente();
        char hand[5];
        strncpy(hand, "l0000", 5);
        handshake(hand, idCliente);
        if(consultarOcupacionHeladeria(idCliente)) {
            printf("Cliente %ld: Hay demasiada gente, me voy\n", idCliente);
            return 0;
        }
        realizarPedido(idCliente);
        retirarPedido(idCliente);
        return 0;
    } else {
        return pid;
    }
}

void realizarPedido(long idCliente) {
    char pedido[4];
    getPedido(pedido);
    printf("Cliente %ld: Envio pedido a cajero: %s\n", idCliente, pedido);
    enviarPedidoCajero(pedido, idCliente);
    char ticket[4];
    printf("Cliete %ld: Espero ticket del cajero.\n", idCliente);
    recibirTicket(ticket, idCliente);
    enviarDesocupacionCaja(idCliente);
    printf("Cliente %ld: Libero lugar en cola.\n", idCliente);
}

void retirarPedido(long idCliente) {
    Message msgRcv;
    printf("Cliente %ld: Estoy esperando mi pedido.\n", idCliente);
    char pedido[4];
    recibirHelado(pedido, idCliente);
    printf("Cliente %ld: Recibi mi pedido.\n", idCliente);
    if(pedido[3] == LLEVAR) {
        printf("Cliente %ld: Es para llevar, me fui.\n", idCliente);
        return;
    }
    printf("Cliente %ld: Como el helado adentro.\n", idCliente);
    esperarAleatorio();
    enviarDesocupacionHeladeria(idCliente);
    printf("Cliente %ld: Libero lugar y me voy.\n", idCliente);
}

void getPedido(char* pedido) {
    char pedidoNuevo[4];
    pedidoNuevo[0] = numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[1] = numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[2]= numeroAleatorioRango(1, 7) + '0';
    pedidoNuevo[3] = numeroAleatorioRango(1, 2) + '0';

    strncpy(pedido, pedidoNuevo, 4);
}