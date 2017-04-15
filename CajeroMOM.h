//
// Created by fedenote on 4/4/17.
//

#ifndef INC_7574TP1_CAJEROMOM_H
#define INC_7574TP1_CAJEROMOM_H

#include "Middleware.h"

pid_t startCajeroMOM();

void enviarPedidoHeladero(char*, long);

void enviarTicketCliente(char*, long);

void recibirPedidoCliente(char*, long*);

#endif //INC_7574TP1_CAJEROMOM_H
