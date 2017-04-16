//
// Created by fedenote on 4/16/17.
//

#ifndef INC_7574TP1_CLIENTEMOM_H
#define INC_7574TP1_CLIENTEMOM_H

#include "Middleware.h"


pid_t startClienteMOM();

void enviarPedidoCajero(char*, long);

void recibirTicket(char*, long);

void recibirHelado(char*, long);

#endif //INC_7574TP1_CLIENTEMOM_H
