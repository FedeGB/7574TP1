//
// Created by fedenote on 3/12/17.
//

#include "Cliente.h"


pid_t generarClientes(int cantClientes) {
    pid_t pid = fork();
    if(pid == 0) {
        for(int i = 0; i < cantClientes; i++) {

        }
    } else {
        return pid;
    }
}