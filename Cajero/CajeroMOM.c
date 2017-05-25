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
        int clientePort = CLIENTEPORT;
        const char* clienteIP = CLIENTEIP;
        int toClfd = connectTo(queues[1], clientePort, clienteIP);
        if(toClfd < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", clienteIP, clientePort);
        } else {
            printf("Me conecte a %s con puerto %d\n", clienteIP, clientePort);
        }
        int heladeroPort = HELADEROPORT;
        const char* heladeroIP = HELADEROIP;
        int toHlfd = connectTo(queues[5], heladeroPort, heladeroIP);
        if(toHlfd  < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", heladeroIP, heladeroPort);
        } else {
            printf("Me conecte a %s con puerto %d\n", heladeroIP, heladeroPort);
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
        working = work(toHl, queues[5], true);
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
    strncpy(msgSend.data, pedido, 4);
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
    strncpy(msgSend.data, ticket, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool recibirPedidoCliente(char* pedido, long* idRcv) {
    int output = getmsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
    if(output < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(output, &msgRcv, sizeof(msgRcv), 0) < 0) {
        return false;
    }
    strncpy(pedido, msgRcv.data, 4);
    *idRcv = msgRcv.mtype;
    return true;
}

long registrarCajero() {
    int regIn = getmsg(QREGISTROCAJINID, QREGISTROCAJINPATH);
    int regOut = getmsg(QREGISTROCAJOUTID, QREGISTROCAJOUTPATH);
    Message regMsg;
    regMsg.mtype = getpid();
    strncpy(regMsg.data, "rrrr", 4);
    enviarmsg(regIn, &regMsg, sizeof(regMsg));
    Message regRcv;
    if(recibirmsg(regOut, &regRcv, sizeof(regRcv), getpid()) >= 0) {
        return regRcv.mtype;
    }
    return -1;
}
