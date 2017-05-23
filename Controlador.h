//
// Created by fedenote on 3/28/17.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "IPC/ColaMensajes.h"
#include "Cajero.h"
#include "CajeroMOM.h"
#include "ClienteMOM.h"
#include "HeladeroMOM.h"
#include "Heladero.h"
#include "Cliente.h"
#include <vector>
#include "IPC/Socket.h"

#ifndef INC_7574TP1_CONTROLADOR_H
#define INC_7574TP1_CONTROLADOR_H

int start(int*, int*, int*, pid_t*, pid_t*, pid_t*, int);

void iniciarIPCs(int*, int*, int*, int);

void iniciarSharedMemories(int*, int*);

bool iniciarTrabajadores(pid_t*, pid_t*, pid_t*, int*, int);

pid_t simular(int*, int*, int*, pid_t*, pid_t*, pid_t*);

void cerrarIPCs(int*, int*, int*);

#endif //INC_7574TP1_CONTROLADOR_H
