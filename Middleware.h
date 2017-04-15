//
// Created by fedenote on 4/2/17.
//

#ifndef INC_7574TP1_MIDDLEWARE_H
#define INC_7574TP1_MIDDLEWARE_H

#include <unistd.h>
#include <sys/wait.h>
#include "ColaMensajes.h"
#include "Semaforo.h"
#include "Constantes.h"
#include <vector>

typedef struct colaPedido {
    int qId;
    const char* qPath;
} qPedido;

pid_t startMiddleWare(qPedido*, int, qPedido*);

pid_t work(int, int);

pid_t registerer(int, int);

#endif //INC_7574TP1_MIDDLEWARE_H
