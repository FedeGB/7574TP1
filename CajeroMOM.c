//
// Created by fedenote on 4/4/17.
//

#include "CajeroMOM.h"



pid_t startCajeroMOM() {
    qPedido colasMiddle[6];
    qPedido registro[2];
    colasMiddle[0].qId= QCAJEROTOCLID;
    colasMiddle[0].qPath = QCAJEROTOCLPATH;
    colasMiddle[1].qId = QTOCLIENTECJID;
    colasMiddle[1].qPath = QTOCLIENTECJPATH;
    colasMiddle[2].qId = QCLIENTETOCJID;
    colasMiddle[2].qPath = QCLIENTETOCJPATH;
    colasMiddle[3].qId = QFROMCLIENTECJID;
    colasMiddle[3].qPath = QFROMCLIENTECJPATH;
    colasMiddle[4].qId = QTOHELADEROCJID;
    colasMiddle[4].qPath = QTOHELADEROCJPATH;
    colasMiddle[5].qId = QCAJEROTOHELID;
    colasMiddle[5].qPath = QCAJEROTOHELPATH;
    registro[0].qId = QREGISTROCAJINID;
    registro[0].qPath = QREGISTROCAJINPATH;
    registro[1].qId = QREGISTROCAJOUTID;
    registro[1].qPath = QREGISTROCAJOUTPATH;
    return startMiddleWare(colasMiddle, 6, registro);
}

// TEngo que hacer un MOM para cliente que hable con el mom del cajero y el del heladero
// Cola unica de entrada MOM. Salida depende de a que MOM se quiera comunicar?
// Tambien deberia haber un enviar al heladero aca

void enviarPedidoHeladero(char* pedido, long id) {
    int queue = getmsg(QTOHELADEROCJID, QTOHELADEROCJPATH);
    Message msgSend;
    msgSend.mtype = id;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
}

void enviarTicketCliente(char* ticket, long idCl) {
    int queue = getmsg(QCAJEROTOCLID, QCAJEROTOCLPATH);
    Message msgSend;
    msgSend.mtype = idCl;
    strncpy(msgSend.data, ticket, 4);
    enviarmsg(queue, &msgSend, sizeof(msgSend));
}

void recibirPedidoCliente(char* pedido, long* idRcv) {
    int output = getmsg(QFROMCLIENTECJID, QFROMCLIENTECJPATH);
    Message msgRcv;
    recibirmsg(output, &msgRcv, sizeof(msgRcv), 0);
    strncpy(pedido, msgRcv.data, 4);
    *idRcv = msgRcv.mtype;
}
