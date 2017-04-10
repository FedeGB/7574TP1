//
// Created by fedenote on 4/4/17.
//

#include "CajeroMOM.h"



pid_t startCajeroMOM() {
    return startMiddleWare(QREGISTROCAJINID, QREGISTROCAJINPATH
            , QREGISTROCAJOUTID, QREGISTROCAJOUTPATH, QCAJEROPATHIN, QCAJEROIDIN, QCAJEROPATHOUT, QCAJEROIDOUT);
}

void enviarPedido(char* pedido, int id) {
    int input = getmsg(QCAJEROIDIN, QCAJEROPATHIN);
    Message msgSend;
    msgSend.mtype = id;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(input, &msgSend, sizeof(msgSend));
}

void recibirPedido(char* pedido, int* idRcv) {
    int output = getmsg(QCAJEROIDOUT, QCAJEROPATHOUT);
    Message msgRcv;
    recibirmsg(output, &msgRcv, sizeof(msgRcv), 0);
    strncpy(pedido, msgRcv.data, 4);
    *idRcv = msgRcv.mtype;
}