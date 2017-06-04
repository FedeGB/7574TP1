//
// Created by fedenote on 5/26/17.
//

#ifndef INC_7574TP1_BROKER_H
#define INC_7574TP1_BROKER_H

#include <stdio.h>
#include <unistd.h>
#include "../IPC/Socket.h"
#include "../IPC/ColaMensajes.h"
#include "Constantes.h"


pid_t startListener(int);

pid_t atenderConexion(int);

pid_t startRouter(int, int);

bool registrarEntidad(MessageInternal);

bool routearMensaje(MessageInternal, int);

pid_t atenderGusto(MessageInternal, int);

pid_t startPoteAdmin(int, int);

pid_t startHeladeriaAdmin(int, int);

bool ocupacionHeladeria();

#endif //INC_7574TP1_BROKER_H
