//
// Created by fedenote on 4/16/17.
//

#include "ClienteMOM.h"


pid_t startClienteMOM() {
    qPedido colasMiddle[6];
    qPedido registro[2];
    colasMiddle[0].qId = QTOCAJEROCLID;
    colasMiddle[0].qPath = QTOCAJEROCLPATH;
    colasMiddle[1].qId = QCLIENTETOCJID;
    colasMiddle[1].qPath = QCLIENTETOCJPATH;
    colasMiddle[2].qId = QCAJEROTOCLID;
    colasMiddle[2].qPath = QCAJEROTOCLPATH;
    colasMiddle[3].qId = QFROMCAJEROCLID;
    colasMiddle[3].qPath = QFROMCAJEROCLPATH;
    colasMiddle[4].qId = QHELADEROTOCLID;
    colasMiddle[4].qPath = QHELADEROTOCLPATH;
    colasMiddle[5].qId = QFROMHELADEROCLID;
    colasMiddle[5].qPath = QFROMHELADEROCLPATH;
    registro[0].qId = QREGISTROCLINID;
    registro[0].qPath = QREGISTROCLINPATH;
    registro[1].qId = QREGISTROCLOUTID;
    registro[1].qPath = QREGISTROCLOUTPATH;
    return startMiddleWare(colasMiddle, 6, registro);
}

void enviarPedidoCajero(char* pedido, long idCliente) {
    int queue = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    Message msgSend;
    msgSend.mtype = idCliente;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
}

void recibirTicket(char* pedido, long idCliente) {
    int queue = getmsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    Message msgRcv;
    recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente);
    strncpy(pedido, msgRcv.data, 4);
}

void recibirHelado(char* pedido, long idCliente) {
    int queue = getmsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
    Message msgRcv;
    recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente);
    strncpy(pedido, msgRcv.data, 4);
}