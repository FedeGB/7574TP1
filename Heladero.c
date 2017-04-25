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
        bool flag = true;
        while(flag) {
            flag = atenderPedido();
        }
        printf("Heladero %d: Termine de atender\n", getpid());
        return 0;
    } else {
        return proceso;
    }
}

bool atenderPedido() {
    printf("Heladero %d: Espero pedido.\n", getpid());
    char pedido[4];
    long idCliente;
    if(!recibirPedidoCajero(pedido, &idCliente)) {
        return false;
    }
    printf("Heladero %d: Recibi pedido.\n", getpid());
    prepararHelado(pedido);
    printf("Heladero %d: Devuelvo helado a cliente.\n", getpid());
    if(!devolverPedidoCliente(pedido, idCliente)) {
        return false;
    }
    return true;
}

void prepararHelado(char* gustos) {
    int semGusto = 0;
    printf("Heladero %d: Preparo helado.\n", getpid());
    char gusto;
    for(int i = 0; i < 3; i++) {
        gusto = gustos[i];
        semGusto = getSemaforo(gusto, SEMGUSTOS);
        printf("Pongo %c\n", gusto);
        p(semGusto);
        esperarAleatorio();
        v(semGusto);
    }
}