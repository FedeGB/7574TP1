//
// Created by fedenote on 4/4/17.
//

#include "CajeroMOM.h"



pid_t startCajeroMOM() {
    return startMiddleWare(QREGISTROCAJINID, QREGISTROCAJINPATH
            , QREGISTROCAJOUTID, QREGISTROCAJOUTPATH, QCAJEROPATHIN, QCAJEROIDIN, QCAJEROPATHOUT, QCAJEROIDOUT);
}

//void enviarPedidoStr(int input, char* pedido, int id) {
//    Message msgSend;
//    msgSend.mtype = id;
//    strncpy(msgSend.data, pedido, 4);
//    enviarmsg(input, &msgSend, sizeof(msgSend));
//}
//
//void enviarPedidoMsg(int input, Message msg) {
//    enviarmsg(input, &msg, sizeof(msg));
//}
//
//void recibirPedido(int output, char* pedido, int* idRcv) {
//    Message msgRcv;
//    recibirmsg(output, &msgRcv, sizeof(msgRcv));
//    strncpy(pedido, msgRcv.data, 4);
//    *idRcv = msgRcv.mtype;
//}