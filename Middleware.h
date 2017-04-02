//
// Created by fedenote on 4/2/17.
//

#ifndef INC_7574TP1_MIDDLEWARE_H
#define INC_7574TP1_MIDDLEWARE_H

#include <unistd.h>
#include "ColaMensajes.h"
#include "Semaforo.h"
#include "Constantes.h"

pid_t startMiddleWare(int*, int*, char*, int, char*, int);

void work(int, int);

pid_t startCajero(int*, int*);

void enviarPedido(int, char*, int);

void recibirPedido(int, char*, int*);

#endif //INC_7574TP1_MIDDLEWARE_H
