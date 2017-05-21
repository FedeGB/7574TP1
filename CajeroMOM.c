//
// Created by fedenote on 4/4/17.
//

#include "CajeroMOM.h"



pid_t startCajeroMOM(int* queues) {
    qPedido colasMiddle[6];
    qPedido registro[2];
    colasMiddle[0].qId = QTOCLIENTECJID;
    colasMiddle[0].qPath = QTOCLIENTECJPATH;
    colasMiddle[0].isSocket = false;
//    colasMiddle[1].qId= QCAJEROTOCLID;
//    colasMiddle[1].qPath = QCAJEROTOCLPATH;
    colasMiddle[1].qId = queues[1];
    colasMiddle[1].isSocket = true;
    colasMiddle[1].doConnect = true;
    colasMiddle[1].ip = "127.0.0.1";
    colasMiddle[1].port = 8081;
//    colasMiddle[2].qId = QCLIENTETOCJID;
//    colasMiddle[2].qPath = QCLIENTETOCJPATH;
    colasMiddle[2].qId = queues[2];
    colasMiddle[2].isSocket = true;
    colasMiddle[2].doReceive = false;
    colasMiddle[3].qId = QFROMCLIENTECJID;
    colasMiddle[3].qPath = QFROMCLIENTECJPATH;
    colasMiddle[3].isSocket = false;
    colasMiddle[4].qId = QTOHELADEROCJID;
    colasMiddle[4].qPath = QTOHELADEROCJPATH;
    colasMiddle[4].isSocket = false;
//    colasMiddle[5].qId = QCAJEROTOHELID;
//    colasMiddle[5].qPath = QCAJEROTOHELPATH;
    colasMiddle[5].qId = queues[5];
    colasMiddle[5].isSocket = true;
    colasMiddle[5].ip = "127.0.0.1";
    colasMiddle[5].port = 8083;
    colasMiddle[5].doConnect = true;
    registro[0].qId = QREGISTROCAJINID;
    registro[0].qPath = QREGISTROCAJINPATH;
    registro[1].qId = QREGISTROCAJOUTID;
    registro[1].qPath = QREGISTROCAJOUTPATH;
    return startMiddleWare(colasMiddle, 6, registro);
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
    int queue = getmsg(QCAJEROTOCLID, QCAJEROTOCLPATH);
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
