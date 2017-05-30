//
// Created by fedenote on 4/4/17.
//

#ifndef INC_7574TP1_CAJEROMOM_H
#define INC_7574TP1_CAJEROMOM_H

#include "../Common/Middleware.h"
#include "Constantes.h"

pid_t startCajeroMOM(int*);

bool enviarPedidoHeladero(char*, long);

bool enviarTicketCliente(char*, long);

bool recibirPedidoCliente(char*, long*);

long registrarCajero();

#endif //INC_7574TP1_CAJEROMOM_H