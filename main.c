//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"

int main() {
    printf("Comienza heladeria\n");
    int queues[3], sharedMem[3], semaforos[11];
    pid_t cajero;
    pid_t heladeros[2];

    int controlador = start(queues, sharedMem, semaforos, &cajero,  heladeros);
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
    bool* entrada = (bool*)map(sharedMem[2]);
    *entrada = false;
    unmap(entrada);
    v(semaforos[10]);

    p(semaforos[0]);
    int* lugaresH = (int*)map(sharedMem[0]);
    *lugaresH = 0;
    unmap(lugaresH);
    v(semaforos[0]);
    p(semaforos[9]);
    int* lugaresC = (int*)map(sharedMem[1]);
    *lugaresC = MAXCOLACAJER + 100;
    unmap(lugaresC);
    v(semaforos[9]);

    char cierre[4];
    Message msg;
    getPedidoCierre(cierre);
    msg.mtype = 1000000;
    strncpy(msg.data, cierre, 4);

    printf("Envio mensaje de cierre a cajero y heladeros\n");
    p(semaforos[1]);
    enviarmsg(queues[0],&msg, sizeof(msg));
    waitpid(cajero, NULL, 0);
    printf("Llego cajero\n");
    for(int hel = 0; hel < 2; hel++) {
        waitpid(heladeros[hel], NULL, 0);
        printf("Llego heladero\n");
    }
    for(std::vector<pid_t>::iterator it = pid_clientes.begin(); it != pid_clientes.end(); it++) {
        waitpid(*it, NULL, 0);
        printf("Llego clinete\n");
    }
    cerrarIPCs(queues, sharedMem, semaforos);
    return 0;
}