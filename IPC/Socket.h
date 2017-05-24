//
// Created by fedenote on 5/1/17.
//

#ifndef INC_7574TP1_SOCKET_H
#define INC_7574TP1_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ColaMensajes.h"
#include <string>
#include "../Common/Utilities.h"

int createSocket(const char*, int, bool);

int receiveConnection(int, struct sockaddr*, socklen_t*);

int connectTo(int, int, const char*);

int receiveFrom(int, Message*);

int sendTo(int, Message*, int);

#endif //INC_7574TP1_SOCKET_H