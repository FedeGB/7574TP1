//
// Created by fedenote on 5/26/17.
//

#ifndef INC_7574TP1_CONTROLADOR_H
#define INC_7574TP1_CONTROLADOR_H

#include "Broker.h"
#include "Constantes.h"

pid_t initBroker(int*, int*, int*, pid_t*, pid_t*);

void inicializarIPCS(int*, int*, int*);

bool inicializarTrabajadores(int*, pid_t*, pid_t*);

void cerrarIPCs(int*, int*, int*);

#endif //INC_7574TP1_CONTROLADOR_H
