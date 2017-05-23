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
#include <string>

//int* parsearPedido(char* pedido) {
//
//}

void esperarAleatorio();

int numeroAleatorioRango(int, int);

void appendString(const char*, const char*, char*, int);

#endif //INC_7574TP1_UTILITIES_H
