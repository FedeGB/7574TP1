//
// Created by fedenote on 5/26/17.
//


#include "Controlador.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
    int queues[7];
    int sharedmem[5];
    int semaforos[11];
    pid_t listener;
    pid_t router;
    pid_t adminPotes;
    pid_t adminHel;
    pid_t init = initBroker(queues, sharedmem, semaforos, &listener, &router, &adminPotes, &adminHel);
    if(init == 0) {
        return 0;
    }
    // TODO: Hago un getinput como en los otros y si aparece x cierro IPCS y dejo que muera todo?
    waitpid(init, NULL, 0);
    // TODO: Necesito esperar por estos dos ultimos realmente?
    waitpid(listener, NULL, 0);
    waitpid(router, NULL, 0);
    waitpid(adminPotes, NULL, 0);
    waitpid(adminHel, NULL, 0);
    cerrarIPCs(queues, sharedmem, semaforos);

    return 0;
}