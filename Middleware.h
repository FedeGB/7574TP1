//
// Created by fedenote on 4/2/17.
//

#ifndef INC_7574TP1_MIDDLEWARE_H
#define INC_7574TP1_MIDDLEWARE_H

#include <unistd.h>
#include "ColaMensajes.h"
#include "Semaforo.h"
#include "Constantes.h"

pid_t startMiddleWare(int, char*, int, char*, char*, int, char*, int);

pid_t work(int, int);

pid_t registerer(int, int);

#endif //INC_7574TP1_MIDDLEWARE_H
