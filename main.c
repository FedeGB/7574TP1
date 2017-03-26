//
// Created by fedenote on 3/13/17.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "ColaMensajes.h"
#include "Cajero.h"
#include "Heladero.h"
#include "Cliente.h"

int main() {
    printf("Comienza heladeria\n");
    int queueCajero = creamsg(QCAJEROID, QCAJEROPATH);
    int queueHeladeros = creamsg(QHELADEROID, QHELADEROPATH);
    int queueRetirar = creamsg(QRETIRARID, QRETIRARPATH);
    int lugaresHeladeria = creashm(LUGARESID, sizeof(int), LUGARESPATH);
    int lugaresCajero = creashm(LUGARESCAJEROID, sizeof(int), LUGARESCAJEROPATH);
    int entradaShm = creashm(ENTRADAID, sizeof(bool), ENTRADAPATH);
    int semLugares = crearSemaforo(SEMLUGARESPATH, SEMLUGARESID, 2);
    int semCajero = crearSemaforo(SEMCAJEROPATH, SEMCAJEROID, 0);
    int semVainilla = crearSemaforo(SEMGUSTOS, VAINILLA, 1);
    int semFrutilla = crearSemaforo(SEMGUSTOS, FRUTILLA, 1);
    int semDulce = crearSemaforo(SEMGUSTOS, DULCEDELECHE, 1);
    int semLimon = crearSemaforo(SEMGUSTOS, LIMON, 1);
    int semSambayon = crearSemaforo(SEMGUSTOS, SAMBAYON, 1);
    int semCrema = crearSemaforo(SEMGUSTOS, CREMAAMERICANA, 1);
    int semMenta = crearSemaforo(SEMGUSTOS, MENTAGRANIZADA, 1);
    int semLugaresCaj = crearSemaforo(SEMLUGARESCAJPATH, SEMLUGARESCAJID, 1);
    int semEntrada = crearSemaforo(SEMENTRADAPATH, SEMENTRADAID, 1);
    printf("Se generaron ipcs.\n");

    p(semLugares);
    int* lugaresH = (int*)map(lugaresHeladeria);
    *lugaresH = ESPACIOHELADERIA;
    unmap(lugaresH);
    v(semLugares);
    p(semEntrada);
    bool* entrada = (bool*)map(entradaShm);
    (*entrada) = true;
    unmap(entrada);
    v(semEntrada);
    printf("Se incializaron variables de memoria compartida.\n");

    pid_t cajero;
    pid_t heladero1;
    pid_t heladero2;
    char input = '1';
    cajero = crearCajero();
    printf("Se creo cajero\n");
    if(cajero == 0) {
        return 0;
    }

    heladero1 = crearHeladero();
    printf("Se creo heladero 1\n");
    if (heladero1 == 0) {
        return 0;
    }
    heladero2 = crearHeladero();
    printf("Se creo heladero 2\n");
    if (heladero2 == 0) {
        return 0;
    }


    while(input != 'x') {
        input = getchar();
        if(input == 'c'){
            pid_t cliente = generarCliente();
            if(cliente == 0) {
                return 0;
            } else if (cliente > 0) {
                printf("Se creo cliente.\n");
//                pid_clientes.push_back(cliente);
            }
        }
    }

    printf("Se termino, esperando a que terminen todos.\n");
    p(semEntrada);
    entrada = (bool*)map(entradaShm);
    *entrada = false;
    unmap(entrada);
    v(semEntrada);

    char cierre[4];
    Message msg;
    getPedidoCierre(cierre);
    msg.mtype = getpid();
    strncpy(msg.data, cierre, 4);
    printf("Envio mensaje de cierre a cajero y heladeros");
    enviarmsg(queueCajero,&msg, sizeof(msg));
    waitpid(cajero, NULL, 0);
    elimsg(queueCajero);
    waitpid(heladero1, NULL, 0);
    waitpid(heladero2, NULL, 0);
    elimsg(queueHeladeros);

    elimsg(queueRetirar);
    elishm(lugaresHeladeria);
    elishm(lugaresCajero);
    elishm(entradaShm);
    eliminarSemaforo(semLugaresCaj);
    eliminarSemaforo(semLugares);
    eliminarSemaforo(semCajero);
    eliminarSemaforo(semVainilla);
    eliminarSemaforo(semFrutilla);
    eliminarSemaforo(semDulce);
    eliminarSemaforo(semLimon);
    eliminarSemaforo(semSambayon);
    eliminarSemaforo(semCrema);
    eliminarSemaforo(semMenta);
    eliminarSemaforo(semEntrada);
    return 0;
}