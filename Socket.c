//
// Created by fedenote on 5/1/17.
//

#include "Socket.h"

int createSocket(const char* ip, int puerto, bool bind) {
    int sfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("Error al obtener socket descriptor");
        return -1;
    }
    if(bind) {
        struct sockaddr_in direccion_sev;
        direccion_sev.sin_family = AF_INET;
        direccion_sev.sin_port = puerto;
        direccion_sev.sin_addr.s_addr = inet_addr(ip);

        int binder = ::bind(sfd, (struct sockaddr*)&direccion_sev, sizeof(struct sockaddr_in));
        if(binder < 0) {
            perror("Error al bindear\n");
            close(sfd);
            return -1;
        }

        int listener = listen(sfd, 10);

        if(listener < 0) {
            perror("Error en el listener");
            close(sfd);
            return -1;
        }
    }
    return sfd;
}

int receiveConnection(int sfd, struct sockaddr* clientAddr, unsigned int* longitudCliente) {
    int clientSfd = accept(sfd, clientAddr, longitudCliente);
    return clientSfd;
}

int connectTo(int sfd, int port, const char* ip) {
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = port;
    direccion.sin_addr.s_addr = inet_addr(ip);

    int connecter = connect(sfd, (struct sockaddr*)&direccion, sizeof(direccion));
    if(connecter < 0) {
        perror("Error al conectar");
        close(sfd);
        return -1;
    }
    return connecter;
}

int receiveFrom(int sfd, Message* message) {
    int lectura = 0;
    int readStatus = 0;
    char buffer[10];
    char bufferTmp[10];
    strcpy(buffer, "");
    strcpy(bufferTmp, "") ;
    while(lectura < 10) {
        readStatus = read(sfd, &buffer, sizeof(buffer));
        if(readStatus < 0) {
            return -1;
        }
        lectura += readStatus;
        strncat(buffer, bufferTmp, readStatus);
        strcpy(bufferTmp, "");
    }
    strncpy(message->data, buffer, 4);
    std::string StrTmp(buffer);
    std::string number = StrTmp.substr(4, StrTmp.find(".") - 4);
    message->mtype = std::stol(number);
    return lectura;
}


int sendTo(int sfd, char* message, int size) {
    int writeStatus = 0;
    writeStatus = write(sfd, &message, size);
    if (writeStatus < 0) {
        return -1;
    }
    return 0;
}