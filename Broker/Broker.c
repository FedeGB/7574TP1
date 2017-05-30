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
        int outputQ = getmsg(QOUTGOINGID, QOUTGOINGPATH);
        if(handShake.data[1] == '0' && handShake.data[2] == '0' && handShake.data[3] == '0') {
            MessageInternal registering;
            strncpy(registering.data, handShake.data, 1); // First char is h, j or l
            char buffer[10];
            std::string number = std::to_string(handShake.mtype);
            appendString(registering.data, number.c_str(), buffer, 10);
            registering.mtype = getpid();
            enviarmsg(inputQ, &registering, sizeof(registering));
        }
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
        int inputQ = getmsg(QINCOMINGID, QINCOMINGPATH);
        int outputQ = getmsg(QOUTGOINGID, QOUTGOINGPATH);
        while(true) {
            MessageInternal internalRcv;
            if (recibirmsg(inputQ, &internalRcv, sizeof(internalRcv)) < 0) {
                printf("No se pudo obtener mensaje de cola input\n");
                return 0;
            }
            if (internalRcv.data[0] == 'h' || internalRcv.data[0] == 'j' || internalRcv.data[0] == 'l') {
                if(!registrarEntidad(internalRcv)) {
                    printf("No se pudo registar a %s\n", internalRcv.data);
                    return 0;
                }
            }
            if(!routearMensaje(internalRcv, outputQ)) {
                printf("No se pudo routear el mensaje %s", internalRcv.data);
                return 0;
            }
        }
    } else {
        return router;
    }
}


bool registrarEntidad(MessageInternal msg) {
    char type = msg.data[0];
    std::string datos(msg.data);
    std::string idStr = datos.substr(1, datos.find(".") - 1);
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
            entidades[*cant] = nuevaEntidad;
            *cant++;
            unmap(cant);
            unmap(entidades);
            v(sem);
        } else {
            return false;
        }
    } else {
        return false;
    };
    printf("Entidad registrada.");
    return true;
}


bool routearMensaje(MessageInternal msg, int output) {

    return true;
}