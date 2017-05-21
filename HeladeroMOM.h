//
// Created by fedenote on 4/10/17.
//

#ifndef INC_7574TP1_HELADEROMOM_H
#define INC_7574TP1_HELADEROMOM_H

#include "Middleware.h"

pid_t startHeladeroMOM(int*);

bool devolverPedidoCliente(char*, long);

bool recibirPedidoCajero(char*, long*);

long registrarHeladero();

#endif //INC_7574TP1_HELADEROMOM_H
