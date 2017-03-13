//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_SEMAFORO_H
#define INC_7574TP1_SEMAFORO_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

struct Semaforo {
    int id;
    int valorInicial;
} semaforo_st;


semaforo_st crearSemaforo ( const char*, const int);

int inicializar (int , int);

// Restar al semaforo
int p (const semaforo_st);

// Sumar al semaforo
int v (const semaforo_st);

void eliminar (const semaforo_st);

#endif //INC_7574TP1_SEMAFORO_H
