//
// Created by fedenote on 4/10/17.
//

#ifndef INC_7574TP1_HELADEROMOM_H
#define INC_7574TP1_HELADEROMOM_H

#include "Middleware.h"

pid_t startHeladeroCajeroMOM();

void devolverPedido(char*, long);

void recibirPedido(char*, long*);

void recibirPedidoHeladero(char*, long);

#endif //INC_7574TP1_HELADEROMOM_H
