//
// Created by fedenote on 3/18/17.
//

#ifndef INC_7574TP1_CAJERO_H
#define INC_7574TP1_CAJERO_H

#include <stdio.h>
#include <unistd.h>
#include "Semaforo.h"
#include "ColaMensajes.h"
#include "Utilities.h"

pid_t crearCajero(int, int, int);

void atenderCajero(int, int, int);

#endif //INC_7574TP1_CAJERO_H
