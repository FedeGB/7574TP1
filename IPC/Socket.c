//
// Created by fedenote on 5/1/17.
//

#include "Socket.h"

int createSocket(const char* ip, int puerto, bool bind) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("Error al obtener socket descriptor");
        return -1;
    }
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }
    if(bind) {
        struct sockaddr_in direccion_sev;
        memset(&direccion_sev, 0, sizeof(direccion_sev));
        direccion_sev.sin_family = AF_INET;
        direccion_sev.sin_port = htons(puerto);
        direccion_sev.sin_addr.s_addr = htonl(INADDR_ANY);
        int binder = ::bind(sfd, (struct sockaddr*)&direccion_sev, sizeof(struct sockaddr));
        if(binder < 0) {
            perror("Error al bindear\n");
            close(sfd);
            return -1;
        }
        int listener = listen(sfd, 20);

        if(listener < 0) {
            perror("Error en el listener");
            close(sfd);
            return -1;
        }
    }
    return sfd;
}

int receiveConnection(int sfd, struct sockaddr* clientAddr, socklen_t* longitudCliente) {
    int clientSfd = accept(sfd, clientAddr, longitudCliente);
    return clientSfd;
}

int connectTo(int sfd, int port, const char* ip) {
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(port);
    direccion.sin_addr.s_addr = inet_addr(ip);

    int connecter = connect(sfd, (struct sockaddr*)&direccion, sizeof(struct sockaddr_in));
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
    std::string buffer;
    char bufferTmp[10];
    buffer.clear();
    strcpy(bufferTmp, "") ;
    while(lectura < 10) {
        readStatus = read(sfd, bufferTmp, sizeof(bufferTmp));
        if(readStatus < 0) {
            perror("Error al leer socket");
            return -1;
        }
        if(readStatus == 0) {
            break;
        }
        lectura += readStatus;
        std::string tmpString(bufferTmp);
        buffer += tmpString;
        strcpy(bufferTmp, "");
    }
    strncpy(message->data, buffer.c_str(), 4);
    std::string StrTmp(buffer);
    std::string number = StrTmp.substr(4, StrTmp.find(".") - 4);
    message->mtype = std::stol(number);
    return lectura;
}


int sendTo(int sfd, Message* message, int size) {
    char buffer[size];
    std::string number = std::to_string(message->mtype);
    appendString(message->data, number.c_str(), buffer, size);
    int writeStatus = 0;
    writeStatus = write(sfd, buffer, size);
    if (writeStatus < 0) {
        return -1;
    }
    return 0;
}
