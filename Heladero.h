//
// Created by fedenote on 3/19/17.
//

#ifndef INC_7574TP1_HELADERO_H
#define INC_7574TP1_HELADERO_H

#include <stdio.h>
#include <unistd.h>
#include "ColaMensajes.h"
#include "Utilities.h"

pid_t crearHeladero();

bool atenderPedido(int, int);

void prepararHelado(char*);

#endif //INC_7574TP1_HELADERO_H
