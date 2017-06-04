//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroMOM(int* queues) {
    pid_t middle = fork();
    if(middle == 0) {
        int regisInput, regisOut;
        regisInput = getmsg(QREGISTROHELINID, QREGISTROHELINPATH);
        regisOut = getmsg(QREGISTROHELOUTID, QREGISTROHELOUTPATH);
        pid_t registering = registerer(regisInput, regisOut);
        if(registerer == 0) {
            return 0;
        }
        std::vector<pid_t> trabajadores;
        pid_t working;
        int brokerPort = BROKERPORT;
        const char* borkerIP = BROKERIP;
        int brfd = connectTo(queues[0], brokerPort, borkerIP);
        if(brfd < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", borkerIP, brokerPort);
        } else {
            printf("Me conecte a %s con puerto %d\n", borkerIP, brokerPort);
        }
        int fromCj = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
        int toCl = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
        working = work(queues[0], fromCj, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toCl, queues[0], true);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        waitpid(registering, NULL, 0);
        for(std::vector<pid_t>::iterator it = trabajadores.begin(); it != trabajadores.end(); it++) {
            waitpid(*it, NULL, 0);
        }
        return 0;
    } else {
        return middle;
    }
}

bool devolverPedidoCliente(char* pedido, long idCliente) {
    int queue = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    if(queue < 0) {
        return false;
    }
    Message msgSend;
    msgSend.mtype = idCliente;
    strncpy(msgSend.data, pedido, 4);
    msgSend.data[4] = 'l';
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool pedirPoteHelado(char gusto, long idHeladero) {
    int queue = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    if(queue < 0) {
        return false;
    }
    Message msgSend;
    msgSend.mtype = idHeladero;
    msgSend.data[0] = 'g';
    msgSend.data[1] = gusto;
    msgSend.data[2] = '0';
    msgSend.data[3] = '0';
    msgSend.data[4] = '0';
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    printf("Pedi pote de gusto %c\n", gusto);
    int queueB = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
    if(queue < 0) {
        return false;
    }
    Message rcvMsg;
    if(recibirmsg(queueB, &rcvMsg, sizeof(rcvMsg), idHeladero) < 0) {
        return false;
    }
    printf("Termine de poner gusto %c\n", gusto);
    return true;
}

bool recibirPedidoCajero(char* pedido, long* idCliente) {
    int queueAviso = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    if(queueAviso < 0) {
        return false;
    }
    Message msgWar;
    msgWar.mtype = 1;
    strncpy(msgWar.data, "r0000", 5);
    enviarmsg(queueAviso, &msgWar, sizeof(msgWar));
    int queue = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
    if(queue < 0) {
        return false;
    }
    Message rcvMsg;
    if(recibirmsg(queue, &rcvMsg, sizeof(rcvMsg), 0) < 0) {
        return false;
    }
    strncpy(pedido, rcvMsg.data, 4);
    *idCliente = rcvMsg.mtype;
    return true;
}

long registrarHeladero() {
    int regIn = getmsg(QREGISTROHELINID, QREGISTROHELINPATH);
    int regOut = getmsg(QREGISTROHELOUTID, QREGISTROHELOUTPATH);
    Message regMsg;
    regMsg.mtype = getpid();
    strncpy(regMsg.data, "rrrrr", 5);
    enviarmsg(regIn, &regMsg, sizeof(regMsg));
    Message regRcv;
    if(recibirmsg(regOut, &regRcv, sizeof(regRcv), getpid()) >= 0) {
        return atol(regRcv.data);
    }
    return -1;
}

bool realizarHandShake(char* entidad, long idRegistro) {
    return devolverPedidoCliente(entidad, idRegistro);
}