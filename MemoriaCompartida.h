//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_MEMORIACOMPARTIDA_H
#define INC_7574TP1_MEMORIACOMPARTIDA_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

typedef struct MemoriaCompartida {
    int shmId;
    void* ptrDatos;
} shm_st;


shm_st* crear(const char*, const char, size_t);

void liberar(shm_st*);

int cantidadProcesosAdosados (shm_st*);

void escribir (shm_st*, void*);

void* leer(shm_st*);

#endif //INC_7574TP1_MEMORIACOMPARTIDA_H
