//
// Created by fedenote on 3/24/17.
//

#include "Utilities.h"

bool seguirAtendiendo() {
    int semEntrada = getSemaforo(SEMENTRADAID, SEMENTRADAPATH);
    int entradaShm = getshm(ENTRADAID, ENTRADAPATH);
    p(semEntrada);
    bool* entrada = (bool*)map(entradaShm);
    bool flag = *entrada;
    unmap(entrada);
    v(semEntrada);
    return flag;
}