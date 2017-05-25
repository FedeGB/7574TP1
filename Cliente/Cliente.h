//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_CLIENTE_H
#define INC_7574TP1_CLIENTE_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../Constantes.h"
#include "../IPC/Semaforo.h"
#include "../IPC/ColaMensajes.h"
#include "../IPC/MemoriaCompartida.h"
#include "../Common/Utilities.h"
#include "ClienteMOM.h"

typedef struct cliente {
    int pid;
} client_st;

pid_t generarCliente();

void realizarPedido(long);

void retirarPedido(long);

void getPedido(char*);

bool todoOcupado();

#endif //INC_7574TP1_CLIENTE_H
