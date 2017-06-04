//
// Created by fedenote on 4/16/17.
//

#ifndef INC_7574TP1_CLIENTEMOM_H
#define INC_7574TP1_CLIENTEMOM_H

#include "../Common/Middleware.h"
#include "Constantes.h"

pid_t startClienteMOM(int*);

bool enviarPedidoCajero(char*, long);

bool recibirTicket(char*, long);

bool recibirHelado(char*, long);

long registrarCliente();

bool handshake(char*, long);

#endif //INC_7574TP1_CLIENTEMOM_H
