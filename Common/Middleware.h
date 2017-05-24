//
// Created by fedenote on 4/2/17.
//

#ifndef INC_7574TP1_MIDDLEWARE_H
#define INC_7574TP1_MIDDLEWARE_H

#include <unistd.h>
#include <sys/wait.h>
#include "../IPC/ColaMensajes.h"
#include "../IPC/Semaforo.h"
#include "../Constantes.h"
#include "../IPC/MemoriaCompartida.h"
#include <vector>
#include "../IPC/Socket.h"
#include "../RPCServer/registerer.h"

pid_t work(int, int, bool);

pid_t registerer(int, int);

int getRegisteringFromRPC();

#endif //INC_7574TP1_MIDDLEWARE_H
