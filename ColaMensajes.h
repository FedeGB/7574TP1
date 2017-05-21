//
// Created by fedenote on 3/18/17.
//

#ifndef INC_7574TP1_COLAMENSAJES_H
#define INC_7574TP1_COLAMENSAJES_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct Message {
    long mtype;
    char data[10];
} Message;

int creamsg(int, const char*);

int getmsg(int, const char*);

void enviarmsg(int, const void*, size_t);

int recibirmsg(int, void*, size_t, long);

int elimsg(int);

#endif //INC_7574TP1_COLAMENSAJES_H
