//
// Created by fedenote on 5/26/17.
//

#include "Controlador.h"


pid_t initBroker(int* queues) {
    pid_t initializer = fork();
    if(initializer == 0) {
        inicializarIPCs(queues);
        return 0;
    } else {
        return initializer;
    }
}


void inicializarIPCS(int* queues) {

}