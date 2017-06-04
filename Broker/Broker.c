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
        printf("Atiendo conexion..\n");
        Message handShake;
        if(receiveFrom(fish, &handShake) < 0) {
            printf("No se pudo recibir nada en handshake.\n");
            return 0;
        }
        printf("Recibi handshake con data: %s\n", handShake.data);
        int inputQ = getmsg(QINCOMINGID, QINCOMINGPATH);
        int outputQ = getmsg(QOUTGOINGID, QOUTGOINGPATH);
        int gustosIn = getmsg(QINGUSTOSID, QINGUSTOSPATH);
        int gustosOut = getmsg(QOUTGUSTOSID, QOUTGUSTOSPATH);
        char type;
        if(handShake.data[1] == '0' && handShake.data[2] == '0' && handShake.data[3] == '0') {
            MessageInternal registering;
            strncpy(registering.data, handShake.data, 5); // First char is h, j or l
            char buffer[10];
            std::string number = std::to_string(handShake.mtype);
            appendString(registering.data, number.c_str(), buffer, 10);
            registering.mtype = getpid();
            strncpy(registering.data, buffer, 10);
            enviarmsg(inputQ, &registering, sizeof(registering));
        }
        // En este primer receive tiene que ver si el clientSocket esta esperando que le devuelvan algo
        // o si esta enviando algo para otro, lo cual condiciona a que este proceso envie el mensaje
        // recibido por el clientSocket al 'inputQ' o que se quede esperando algo en el 'outputQ'
        while(true) {
            Message rcvMsg;
            printf("Espero nuevo pedido\n");
            if(receiveFrom(fish, &rcvMsg) < 0) {
                printf("No se pudo recibir mensaje de cliente.\n");
                return 0;
            }
            if(rcvMsg.data[0] == 'r') {
                printf("Recibi pedido para recibir dato en endpoint\n");
                MessageInternal receivingMessage;
                if(recibirmsg(outputQ, &receivingMessage, sizeof(receivingMessage), getpid()) < 0) {
                    printf("No se pudo obtener mensaje de cola output\n");
                    return 0;
                }
                Message returningMessage;
                strncpy(returningMessage.data, receivingMessage.data, 5);
                std::string dataStr(receivingMessage.data);
                std::string mtype = dataStr.substr(5, dataStr.find('.') - 5);
                returningMessage.mtype = std::stol(mtype);
                sendTo(fish, &returningMessage, 10);
            } else if(rcvMsg.data[0] == 'g') {
                MessageInternal gustoRecibido;
                gustoRecibido.mtype = getpid();
                std::string number = std::to_string(rcvMsg.mtype);
                appendString(rcvMsg.data, number.c_str(), gustoRecibido.data, 10);
                enviarmsg(gustosIn, &gustoRecibido, sizeof(gustoRecibido));
                MessageInternal gustoDeVuelta;
                if(recibirmsg(gustosOut, &gustoDeVuelta, sizeof(gustoDeVuelta), getpid()) < 0) {
                    printf("No se pudo volver a recibir el gusto\n");
                    return 0;
                }
                Message returningMessage;
                strncpy(returningMessage.data, "done0", 5);
                returningMessage.mtype = rcvMsg.mtype;
                sendTo(fish, &returningMessage, 10);
            } else {
                MessageInternal internal;
                internal.mtype = getpid();
                std::string number = std::to_string(rcvMsg.mtype);
                appendString(rcvMsg.data, number.c_str(), internal.data, 10);
                enviarmsg(inputQ, &internal, sizeof(internal));
            }
        }
    } else {
        return newFish;
    }
}

pid_t startRouter(int inputQ, int outputQ) {
    pid_t router = fork();
    if(router == 0) {
//        int inputQ = getmsg(QINCOMINGID, QINCOMINGPATH);
//        int outputQ = getmsg(QOUTGOINGID, QOUTGOINGPATH);
        while(true) {
            MessageInternal internalRcv;
            if (recibirmsg(inputQ, &internalRcv, sizeof(internalRcv), 0) < 0) {
                printf("No se pudo obtener mensaje de cola input\n");
                return 0;
            }
            if (internalRcv.data[0] == 'h' || internalRcv.data[0] == 'j' || internalRcv.data[0] == 'l') {
                if(!registrarEntidad(internalRcv)) {
                    printf("No se pudo registar a %s\n", internalRcv.data);
                    return 0;
                }
            } else if(!routearMensaje(internalRcv, outputQ)) {
                printf("No se pudo routear el mensaje %s", internalRcv.data);
                return 0;
            }
        }
    } else {
        return router;
    }
}


bool registrarEntidad(MessageInternal msg) {
    printf("Recibi entidad para registrar data: %s\n", msg.data);
    char type = msg.data[0];
    std::string datos(msg.data);
    std::string idStr = datos.substr(5, datos.find(".") - 5);
    long id = std::stol(idStr);
    int sem = getSemaforo(SEMTABLEID, SEMTABLEPATH);
    if(sem > 0) {
        int cantTable = getshm(SHAREDCANTID, SHAREDCANTPATH);
        int table = getshm(SHAREDTABLEID, SHAREDTABLEPATH);
        if (cantTable > 0 && table > 0) {
            p(sem);
            int *cant = (int *) map(cantTable);
            entity *entidades = (entity *) map(table);
            // TODO: if cant > max actual, agrandar memoria.. patente pendiente
            entity nuevaEntidad;
            nuevaEntidad.fishPid = msg.mtype;
            nuevaEntidad.id = id;
            nuevaEntidad.type = type;
            nuevaEntidad.weight = 0;
            printf("Guardando entidad de tipo %c con id %ld en en pos %i\n", type, id, *cant);
            entidades[*cant] = nuevaEntidad;
            (*cant)++;
            unmap(cant);
            unmap(entidades);
            v(sem);
            printf("Entidad registrada.\n");
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


bool routearMensaje(MessageInternal msg, int output) {
    int cantTable = getshm(SHAREDCANTID, SHAREDCANTPATH);
    int table = getshm(SHAREDTABLEID, SHAREDTABLEPATH);
    int sem = getSemaforo(SEMTABLEID, SEMTABLEPATH);
    if (cantTable > 0 && table > 0) {
        std::string dataStr(msg.data);
        std::string mtype = dataStr.substr(5, dataStr.find('.') - 5);
        long id = std::stol(mtype);
        long dest = 1;
        p(sem);
        int *cant = (int *) map(cantTable);
        entity *entidades = (entity *) map(table);
        if(msg.data[4] == 'l') {
            for(int i = 0; i < *cant; i++) {
                entity entidad = entidades[i];
                if(entidad.type == msg.data[4] && id == entidad.id) {
                    dest = entidad.fishPid;
                    printf("Encontre nodo destino para cliente.\n");
                    break;
                }
            }
        } else if(msg.data[4] == 'j' || msg.data[4] == 'h') {
            int minWeight = 10000;
            long minWeightDest = 1;
            for(int i = 0; i < *cant; i++) {
                entity entidad = entidades[i];
                if(entidad.type == msg.data[4] && entidad.weight < minWeight) {
                    minWeightDest = entidad.fishPid;
                    minWeight = entidad.weight;
                }
            }
            dest = minWeightDest;
        }
//            unmap(cant);
        unmap(entidades);
        v(sem);
        msg.mtype = dest;
        enviarmsg(output, &msg, sizeof(msg));
    }
    return true;
}

pid_t startPoteAdmin(int input, int output) {
    pid_t poteAdmin = fork();
    if(poteAdmin == 0) {
        while(true) {
            MessageInternal internalRcv;
            if (recibirmsg(input, &internalRcv, sizeof(internalRcv), 0) < 0) {
                printf("No se pudo obtener mensaje de cola input\n");
                return 0;
            }
            if(internalRcv.data[0] == 'g') {
                pid_t gusto = atenderGusto(internalRcv, output);
                if(gusto == 0) {
                    return 0;
                }
            }
        }
    } else {
        return poteAdmin;
    }
}


pid_t atenderGusto(MessageInternal internalRcv, int output) {
    pid_t atender = fork();
    int semGusto;
    if(atender == 0) {
        char gusto = internalRcv.data[1];
        semGusto = getSemaforo(gusto, SEMGUSTOS);
        printf("Se esta usando pote de gusto  %c\n", gusto);
        p(semGusto);
        esperarAleatorio();
        v(semGusto);
        enviarmsg(output, &internalRcv, sizeof(internalRcv));
        return 0;
    } else {
        return atender;
    }
}