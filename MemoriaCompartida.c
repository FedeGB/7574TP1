//
// Created by fedenote on 3/12/17.
//

#include "MemoriaCompartida.h"


shm_st* crear ( const char* archivo, const char letra , size_t size) {
    key_t clave = ftok ( archivo,letra );
    shm_st* memoria = (shm_st*)malloc(sizeof(shm_st));
    if ( clave > 0 ) {
        memoria->shmId = shmget ( clave, size, 0644 | IPC_CREAT );

        if ( memoria->shmId > 0 ) {
            void* tmpPtr = shmat ( memoria->shmId , NULL, 0 );
            if ( tmpPtr != (void*) -1 ) {
                memoria->ptrDatos = tmpPtr;
            } else {
                perror("Error en shmat()");
            }
        } else {
            perror("Error en shmget()");
        }
    } else {
        perror("Error en ftok()");
    }
    return memoria;
}


void liberar(shm_st* memoria) {

    int errorDt = shmdt ( memoria->ptrDatos );

    if ( errorDt != -1 ) {
        int procAdosados = cantidadProcesosAdosados (memoria);
        if ( procAdosados == 0 ) {
            shmctl ( memoria->shmId, IPC_RMID, NULL );
            free(memoria);
        }
    } else {
        perror("Error en shmdt()");
    }
}

int cantidadProcesosAdosados (shm_st* memoria) {
    struct shmid_ds estado;
    shmctl ( memoria->shmId, IPC_STAT, &estado );
    return estado.shm_nattch;
}


void escribir (shm_st* memoria, void* dato ) {
    memoria->ptrDatos = dato;
}

void* leer(shm_st* memoria) {
    return memoria->ptrDatos;
}