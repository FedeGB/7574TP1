//
// Created by fedenote on 4/4/17.
//

#ifndef INC_7574TP1_CAJEROMOM_H
#define INC_7574TP1_CAJEROMOM_H

#include "Middleware.h"

pid_t startCajeroMOM();

bool enviarPedidoHeladero(char*, long);

bool enviarTicketCliente(char*, long);

bool recibirPedidoCliente(char*, long*);

long registrarCajero();

#endif //INC_7574TP1_CAJEROMOM_H
