//
// Created by fedenote on 3/28/17.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "ColaMensajes.h"
#include "Cajero.h"
#include "Heladero.h"
#include "Cliente.h"
#include <vector>

#ifndef INC_7574TP1_CONTROLADOR_H
#define INC_7574TP1_CONTROLADOR_H

pid_t start(int*, int*, int*);
void cerrarIPCs(int*, int*, int*);

#endif //INC_7574TP1_CONTROLADOR_H
