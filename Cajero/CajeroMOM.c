//
// Created by fedenote on 4/4/17.
//

#include "CajeroMOM.h"



pid_t startCajeroMOM(int* queues) {
    pid_t middle = fork();
    if(middle == 0) {
        int regisInput, regisOut;
        regisInput = getmsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
        regisOut = getmsg(QREGISTROCAJOUTID, QREGISTROCAJOUTPATH);
        pid_t registering = registerer(regisInput, regisOut);
        if(registerer == 0) {
            return 0;
        }
        std::vector<pid_t> trabajadores;
        pid_t working;
        int clientePort = BROKERPORT;
        const char* clienteIP = BROKERIP;
        int toClfd = connectTo(queues[1], clientePort, clienteIP);
        if(toClfd < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", clienteIP, clientePort);
        } else {
            printf("Me conecte a %s con puerto %d\n", clienteIP, clientePort);
        }
        int toCl = getmsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
        int fromCl = getmsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
        int toHl = getmsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
        working = work(toCl, queues[1], true);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(queues[1], fromCl, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toHl, queues[1], true);
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

bool enviarPedidoHeladero(char* pedido, long id) {
    int queue = getmsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
    if(queue < 0) {
        return false;
    }
    Message msgSend;
    msgSend.mtype = id;
    strncpy(msgSend.data, pedido, 5);
    msgSend.data[4] = 'h';
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool enviarTicketCliente(char* ticket, long idCl) {
    int queue = getmsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
    if(queue < 0) {
        return false;
    }
    Message msgSend;
    msgSend.mtype = idCl;
    strncpy(msgSend.data, ticket, 5);
    msgSend.data[4] = 'l';
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool recibirPedidoCliente(char* pedido, long* idRcv) {
    int queueAviso = getmsg(QTOCLIENTECJID, QTOCLIENTECJPATH);
    if(queueAviso < 0) {
        return false;
    }
    Message msgWar;
    msgWar.mtype = 1;
    strncpy(msgWar.data, "r0000", 5);
    enviarmsg(queueAviso, &msgWar, sizeof(msgWar));
    int output = getmsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
    if(output < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(output, &msgRcv, sizeof(msgRcv), 0) < 0) {
        return false;
    }
    strncpy(pedido, msgRcv.data, 5);
    *idRcv = msgRcv.mtype;
    return true;
}

long registrarCajero() {
    int regIn = getmsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
    int regOut = getmsg(QREGISTROCAJOUTID, QREGISTROCAJOUTPATH);
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

bool realizarHandShake(char* registro, long idRegistro) {
    return enviarPedidoHeladero(registro, idRegistro);
}
