//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_CLIENTE_H
#define INC_7574TP1_CLIENTE_H

#include <stdio.h>
#include <unistd.h>

typedef struct cliente {
    int pid;
} client_st;

pid_t generarClientes(int cantClientes);



#endif //INC_7574TP1_CLIENTE_H
