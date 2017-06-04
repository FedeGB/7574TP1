//
// Created by fedenote on 3/19/17.
//

#include "Heladero.h"

pid_t crearHeladero() {
    pid_t proceso = fork();
    if(proceso == 0) {
        time_t t;
        srand((unsigned)time(&t));
        // Uso real mas adelante.
        long idHeladero = registrarHeladero();
        char hand[5];
        strncpy(hand, "h0000", 5);
        realizarHandShake(hand, idHeladero);
        bool flag = true;
        while(flag) {
            flag = atenderPedido(idHeladero);
        }
        printf("Heladero %ld: Termine de atender\n", idHeladero);
        return 0;
    } else {
        return proceso;
    }
}

bool atenderPedido(long idHeladero) {
    printf("Heladero %ld: Espero pedido.\n", idHeladero);
    char pedido[4];
    long idCliente/* = idHeladero*/;
    if(!recibirPedidoCajero(pedido, &idCliente)) {
        return false;
    }
    printf("Heladero %ld: Recibi pedido.\n", idHeladero);
    prepararHelado(pedido, idHeladero);
    printf("Heladero %ld: Devuelvo helado a cliente.\n", idHeladero);
    if(!devolverPedidoCliente(pedido, idCliente)) {
        return false;
    }
    return true;
}

void prepararHelado(char* gustos, long idHeladero) {
    printf("Heladero %d: Preparo helado.\n", getpid());
    char gusto;
    for(int i = 0; i < 3; i++) {
        gusto = gustos[i];
        printf("Pongo %c\n", gusto);
        pedirPoteHelado(gusto, idHeladero);
    }
}