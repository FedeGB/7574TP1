//
// Created by fedenote on 3/18/17.
//

#ifndef INC_7574TP1_UTILITIES_H
#define INC_7574TP1_UTILITIES_H

#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "Constantes.h"
#include <time.h>
#include <unistd.h>


//int* parsearPedido(char* pedido) {
//
//}

bool seguirAtendiendo();

void esperarAleatorio();

void getPedidoCierre(char*);

#endif //INC_7574TP1_UTILITIES_H
