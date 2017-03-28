//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"

int main() {
    printf("Comienza heladeria\n");
    int queues[3], sharedMem[3], semaforos[11];
    pid_t controlador = start(queues, sharedMem, semaforos, pid_t cajero, pid_t* heladeros);
    if(controlador == 0) {
        return 0;
    }

    std::vector<pid_t> pid_clientes;
    char input = '1';

    while(input != 'x') {
        input = getchar();
        if(input == 'c'){
            pid_t cliente = generarCliente();
            if(cliente == 0) {
                return 0;
            } else if (cliente > 0) {
                printf("Se creo cliente %d.\n", cliente);
                pid_clientes.push_back(cliente);
            }
        }
    }

    printf("Se termino, esperando a que terminen todos.\n");
    p(semaforos[10]);
    entrada = (bool*)map(sharedMem[2]);
    *entrada = false;
    unmap(entrada);
    v(semaforos[10]);

    p(semaforos[0]);
    lugaresH = (int*)map(sharedMem[0]);
    *lugaresH = 0;
    unmap(lugaresH);
    v(semaforos[0]);
    p(semaforos[9]);
    lugaresC = (int*)map(sharedMem[1]);
    *lugaresC = MAXCOLACAJER;
    unmap(lugaresC);
    v(semaforos[9]);

    char cierre[4];
    Message msg;
    getPedidoCierre(cierre);
    msg.mtype = getpid();
    strncpy(msg.data, cierre, 4);

    printf("Envio mensaje de cierre a cajero y heladeros");
    enviarmsg(queues[0],&msg, sizeof(msg));
    waitpid(cajero, NULL, 0);
    waitpid(heladero1, NULL, 0);
    waitpid(heladero2, NULL, 0);
    for(std::vector<pid_t>::iterator it = pid_clientes.begin(); it != pid_clientes.end(); it++) {
        waitpid(*it, NULL, 0);
    }
    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}