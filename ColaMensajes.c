//
// Created by fedenote on 3/18/17.
//

#include "ColaMensajes.h"

int creamsg(int id, const char* path) {
    key_t clave;
    clave = ftok(path, id);
    int resultado = (msgget(clave,  IPC_CREAT | IPC_EXCL | 0660));
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "No se pudo crear la cola de mensajes con el id %d y path %s", id, path);
        perror(buffer);
    }
    return resultado;
}

int getmsg(int id, const char* path) {
    key_t clave;
    clave = ftok(path, id);
    int resultado = (msgget(clave, 0660));
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "No se pudo obgtener la cola de mensaje con id %d", id);
        perror(buffer);
    }
    return resultado;
}

void enviarmsg(int id, const void *msgp, size_t msgsz) {
    if(msgsnd(id,msgp,msgsz-sizeof(long),0) == -1){
        perror("No se puede enviar el mensaje");
        exit(-1);
    }
}

int recibirmsg(int id, void *msgp, size_t msgsz, long type) {
    int result = msgrcv(id,msgp,msgsz-sizeof(long),type,0);
    if(result == -1){
        perror("No se puede recibir el mensaje");
    }
    return result;
}

int elimsg(int id) {
    int resultado = (msgctl(id, IPC_RMID, NULL));
    if(resultado < 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "No se pudo elimiar la cola de mensajes con id %d", id);
        perror(buffer);
    }
    return resultado;
}
