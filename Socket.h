//
// Created by fedenote on 5/1/17.
//

#ifndef INC_7574TP1_SOCKET_H
#define INC_7574TP1_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ColaMensajes.h"

int createSocket(const char*, int, bool);

int receiveConnection(int, struct sockaddr*, int*);

int receiveFrom(int, Message);

int sendTo(int, char*, int);

#endif //INC_7574TP1_SOCKET_H
