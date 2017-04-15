//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroMOM() {
    qPedido colasMiddle[4];
    qPedido registro[2];
    colasMiddle[0].qId = QCAJEROTOHELID;
    colasMiddle[0].qPath = QCAJEROTOHELPATH;
    colasMiddle[1].qId = QFROMCAJEROHELID;
    colasMiddle[1].qPath = QFROMCAJEROHELPATH;
    colasMiddle[2].qId = QTOCLIENTEHELID;
    colasMiddle[2].qPath = QTOCLIENTEHELPATH;
    colasMiddle[3].qId = QHELADEROTOCLID;
    colasMiddle[3].qPath = QHELADEROTOCLPATH;
    registro[0].qId = QREGISTROHELINID;
    registro[0].qPath = QREGISTROHELINPATH;
    registro[1].qId = QREGISTROHELOUTID;
    registro[1].qPath = QREGISTROHELOUTPATH;
    return startMiddleWare(colasMiddle, 4, registro);
}

void devolverPedidoCliente(char* pedido, long idCliente) {
    int queue = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
    Message msgSend;
    msgSend.mtype = idCliente;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
}

void recibirPedidoCajero(char* pedido, long* idCliente) {
    int queue = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
    Message rcvMsg;
    recibirmsg(queue, &rcvMsg, sizeof(rcvMsg), 0);
    strncpy(pedido, rcvMsg.data, 4);
    *idCliente = rcvMsg.mtype;
}