//
// Created by fedenote on 4/16/17.
//

#include "ClienteMOM.h"


pid_t startClienteMOM(int* queues) {
    qPedido colasMiddle[6];
    qPedido registro[2];
    colasMiddle[0].qId = QTOCAJEROCLID;
    colasMiddle[0].qPath = QTOCAJEROCLPATH;
    colasMiddle[0].isSocket = false;
//    colasMiddle[1].qId = QCLIENTETOCJID;
//    colasMiddle[1].qPath = QCLIENTETOCJPATH;
    colasMiddle[1].qId = queues[2];
    colasMiddle[1].isSocket = true;
//    colasMiddle[2].qId = QCAJEROTOCLID;
//    colasMiddle[2].qPath = QCAJEROTOCLPATH;
    colasMiddle[2].qId = queues[1];
    colasMiddle[2].isSocket = true;
    colasMiddle[3].qId = QFROMCAJEROCLID;
    colasMiddle[3].qPath = QFROMCAJEROCLPATH;
    colasMiddle[3].isSocket = false;
//    colasMiddle[4].qId = QHELADEROTOCLID;
//    colasMiddle[4].qPath = QHELADEROTOCLPATH;
    colasMiddle[4].qId = queues[10];
    colasMiddle[4].isSocket = true;
    colasMiddle[5].qId = QFROMHELADEROCLID;
    colasMiddle[5].qPath = QFROMHELADEROCLPATH;
    colasMiddle[5].isSocket = false;
    registro[0].qId = QREGISTROCLINID;
    registro[0].qPath = QREGISTROCLINPATH;
    registro[1].qId = QREGISTROCLOUTID;
    registro[1].qPath = QREGISTROCLOUTPATH;
    return startMiddleWare(colasMiddle, 6, registro);
}

bool enviarPedidoCajero(char* pedido, long idCliente) {
    int queue = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    if(queue < 0) {
        return false;
    }
    Message msgSend;
    msgSend.mtype = idCliente;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool recibirTicket(char* ticket, long idCliente) {
    int queue = getmsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    if(queue < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente) < 0) {
        return false;
    }
    strncpy(ticket, msgRcv.data, 4);
    return true;
}

bool recibirHelado(char* pedido, long idCliente) {
    int queue = getmsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
    if(queue < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente) < 0) {
        return false;
    }
    strncpy(pedido, msgRcv.data, 4);
    return true;
}

long registrarCliente() {
    int regIn = getmsg(QREGISTROCLINID, QREGISTROCLINPATH);
    int regOut = getmsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
    Message regMsg;
    regMsg.mtype = getpid();
    strncpy(regMsg.data, "rrrr", 4);
    enviarmsg(regIn, &regMsg, sizeof(regMsg));
    Message regRcv;
    if(recibirmsg(regOut, &regRcv, sizeof(regRcv), getpid()) >= 0) {
        return atol(regRcv.data);
    }
    return -1;
}