//
// Created by fedenote on 3/28/17.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "../IPC/ColaMensajes.h"
#include "HeladeroMOM.h"
#include "Heladero.h"
#include <vector>
#include "../IPC/Socket.h"

#ifndef INC_7574TP1_CONTROLADOR_H
#define INC_7574TP1_CONTROLADOR_H

int start(int*, pid_t*, pid_t*);

void iniciarIPCs(int*);

bool iniciarTrabajadores(pid_t*, pid_t*, int*);

pid_t simular();

void cerrarIPCs(int*);

#endif //INC_7574TP1_CONTROLADOR_H
