//
// Created by fedenote on 5/26/17.
//

#include "Broker.h"

pid_t startListener(int socket) {
    pid_t listener = fork();
    if(listener  == 0) {
        int newSFD;
        while(true) {
            struct sockaddr_in clientAddr;
            socklen_t longitudCliente = sizeof(clientAddr);
            printf("Estoy esperando a recibir conexion...\n");
            newSFD = receiveConnection(socket, (struct sockaddr*)&clientAddr, &longitudCliente);
            if(newSFD > 0) {
                printf("Recibi una conexión.\n");
            } else {
                printf("Falle al querer recibir una conexión.\n");
            }
            pid_t newFish = atenderConexion(newSFD);
            if(newFish == 0) {
                return 0;
            }
        }
        return 0;
    } else {
        return listener;
    }
}

pid_t atenderConexion(int fish) {
    pid_t newFish = fork();
    if(newFish == 0) {
        Message handShake;
        if(receiveFrom(fish, &handShake) < 0) {
            printf("No se pudo recibir nada en handshake.\n");
            return 0;
        }
        int inputQ = getmsg(QINCOMINGID, QINCOMINGPATH);
        enviarmsg(inputQ, &handShake, sizeof(handShake));
        int outputQ = getmsg(QOUTGOINGID, QOUTGOINGPATH);
        // En este primer receive tiene que ver si el clientSocket esta esperando que le devuelvan algo
        // o si esta enviando algo para otro, lo cual condiciona a que este proceso envie el mensaje
        // recibido por el clientSocket al 'inputQ' o que se quede esperando algo en el 'outputQ'
        while(true) {
            Message rcvMsg;
            if(receiveFrom(fish, &rcvMsg) < 0) {
                printf("No se pudo recibir mensaje de cliente.\n");
                return 0;
            }

        }
    } else {
        return newFish;
    }
}

pid_t startRouter(int input, int output) {
    pid_t router = fork();
    if(router == 0) {

        return 0;
    } else {
        return router;
    }
}