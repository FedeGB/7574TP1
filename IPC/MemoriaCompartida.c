//
// Created by fedenote on 3/12/17.
//

#include "MemoriaCompartida.h"

int creashm(int id, int size, const char* path) {
    key_t clave;
    clave = ftok(path, id);
    int idShm = (shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660));
    if(idShm < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Fallo al crear memoria compartida con id %d y tamaño %d", id, size);
        perror(buffer);
    }
    return idShm;
}

int getshm(int id, const char* path) {
    key_t clave;
    clave = ftok(path, id);
    int resultado = (shmget(clave, 1, 0660));
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "No se pudo obtener la memoria compartida de id %d", id);
        perror(buffer);
    }
    return resultado;
}

void* map(int id) {
    void*addr = shmat(id, NULL, 0);
    if(addr==(void*)-1){
        perror("No se pudo mapear la memoria");
        return NULL;
    }
    return addr;
}

int unmap(void* addr) {
    int resultado = shmdt(addr);
    if(resultado < 0) {
        perror("No se pudo desattachear la memoria");
    }
    return resultado;
}

int elishm(int id) {
    int resultado = (shmctl(id, IPC_RMID, (struct shmid_ds *) 0));
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "No se pudo eliminar la memoria con id %d", id);
        perror(buffer);
    }
    return resultado;
}