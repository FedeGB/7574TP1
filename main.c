//
// Created by fedenote on 3/13/17.
//

#include "Controlador.h"

int main() {
    printf("Comienza heladeria\n");
    int queues[3], sharedMem[3], semaforos[11];
    pid_t controlador = start(queues, sharedMem, semaforos);
    if(controlador == 0) {
        return 0;
    }
    printf("Se generaron ipcs.\n");

    p(semaforos[0]);
    int* lugaresH = (int*)map(sharedMem[0]);
    *lugaresH = ESPACIOHELADERIA;
    unmap(lugaresH);
    v(semaforos[0]);
    p(semaforos[9]);
    int* lugaresC = (int*)map(sharedMem[1]);
    *lugaresC = 0;
    unmap(lugaresC);
    v(semaforos[9]);
    p(semaforos[10]);
    bool* entrada = (bool*)map(sharedMem[2]);
    (*entrada) = true;
    unmap(entrada);
    v(semaforos[10]);
    printf("Se incializaron variables de memoria compartida.\n");

    pid_t cajero;
    pid_t heladero1;
    pid_t heladero2;
    std::vector<pid_t> pid_clientes;
    char input = '1';
    cajero = crearCajero();
    if(cajero == 0) {
        return 0;
    }
    printf("Se creo cajero\n");

    heladero1 = crearHeladero();
    if (heladero1 == 0) {
        return 0;
    }
    printf("Se creo heladero 1\n");

    heladero2 = crearHeladero();
    if (heladero2 == 0) {
        return 0;
    }
    printf("Se creo heladero 2\n");


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