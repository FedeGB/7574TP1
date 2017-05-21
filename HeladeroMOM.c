//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroMOM(int* queues) {
    qPedido colasMiddle[4];
    qPedido registro[2];
//    colasMiddle[0].qId = QCAJEROTOHELID;
//    colasMiddle[0].qPath = QCAJEROTOHELPATH;
    colasMiddle[0].qId = queues[5];
    colasMiddle[0].isSocket = true;
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
    registro[0].qId = QREGISTROHELINID;
    registro[0].qPath = QREGISTROHELINPATH;
    registro[1].qId = QREGISTROHELOUTID;
    registro[1].qPath = QREGISTROHELOUTPATH;
    return startMiddleWare(colasMiddle, 4, registro);
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