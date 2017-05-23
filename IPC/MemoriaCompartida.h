//
// Created by fedenote on 3/12/17.
//

#ifndef INC_7574TP1_MEMORIACOMPARTIDA_H
#define INC_7574TP1_MEMORIACOMPARTIDA_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int getshm(int, const char*);

int creashm(int, int, const char*);

void* map(int);

int unmap(void*);

int elishm(int);

#endif //INC_7574TP1_MEMORIACOMPARTIDA_H
