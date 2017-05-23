//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroMOM(int* queues) {
    /*qPedido colasMiddle[4];
    qPedido registro[2];
//    colasMiddle[0].qId = QCAJEROTOHELID;
//    colasMiddle[0].qPath = QCAJEROTOHELPATH;
    colasMiddle[0].qId = queues[5];
    colasMiddle[0].isSocket = true;
    colasMiddle[0].doReceive = true;
    colasMiddle[1].qId = QFROMCAJEROHELID;
    colasMiddle[1].qPath = QFROMCAJEROHELPATH;
    colasMiddle[1].isSocket = false;
    colasMiddle[2].qId = QTOCLIENTEHELID;
    colasMiddle[2].qPath = QTOCLIENTEHELPATH;
    colasMiddle[2].isSocket = false;
//    colasMiddle[3].qId = QHELADEROTOCLID;
//    colasMiddle[3].qPath = QHELADEROTOCLPATH;
    colasMiddle[3].qId = queues[10];
    colasMiddle[3].isSocket = true;
    colasMiddle[3].ip = "127.0.0.1";
    colasMiddle[3].port = 8081;
    colasMiddle[3].doConnect = true;
    registro[0].qId = QREGISTROHELINID;
    registro[0].qPath = QREGISTROHELINPATH;
    registro[1].qId = QREGISTROHELOUTID;
    registro[1].qPath = QREGISTROHELOUTPATH;
    return startMiddleWare(colasMiddle, 4, registro);*/
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
        struct sockaddr_in clientAddr;
        socklen_t longitudCliente = sizeof(clientAddr);
        printf("Estoy esperando a recibir conexion...\n");
        int newCjfd = receiveConnection(queues[5], (struct sockaddr*)&clientAddr, &longitudCliente);
        if(newCjfd > 0) {
            printf("Recibi nueva conexión\n");
        } else {
            perror("Fallo en recibir nueva conexión");
        }
        int clientePort = 8081;
        const char* clienteIP = "127.0.0.1";
        int toClfd = connectTo(queues[10], clientePort, clienteIP);
        if(toClfd < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", clienteIP, clientePort);
        } else {
            printf("Me conecte a %s con puerto %d\n", clienteIP, clientePort);
        }
        int fromCj = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
        int toCl = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
        working = work(newCjfd, fromCj, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toCl, queues[10], true);
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
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool recibirPedidoCajero(char* pedido, long* idCliente) {
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
    strncpy(regMsg.data, "rrrr", 4);
    enviarmsg(regIn, &regMsg, sizeof(regMsg));
    Message regRcv;
    if(recibirmsg(regOut, &regRcv, sizeof(regRcv), getpid()) >= 0) {
        return regRcv.mtype;
    }
    return -1;
}