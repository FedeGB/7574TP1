//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_SEMAFORO_H
#define INC_7574TP1_SEMAFORO_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int crearSemaforo ( const char*, int, const int);

int inicializar (int , int);

// Restar al semaforo
int p (const int);

// Sumar al semaforo
int v (const int);

void eliminarSemaforo (const int);

#endif //INC_7574TP1_SEMAFORO_H
