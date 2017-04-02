//
// Created by fedenote on 4/2/17.
//

#include "Middleware.h"


pid_t startMiddleWare(int* input, int* output, char* pathIn, int varIn, char* pathOut, int varOut) {
    pid_t middle = fork();

    if(middle == 0) {
        *input = creamsg(varIn, pathIn);
        *output = creamsg(varOut, pathOut);
        work(*input, *output);
    } else {
        return middle;
    }
}


void work(int input, int output) {
    bool flag = true;
    int status;
    int j = 0;
    int i = 1;
    while(flag) {
        Message msgRcv;
        status = recibirmsg(input, &msgRcv, sizeof(msgRcv), 0);
        if(status >= 0) {
            if(msgRcv.data[0] == '0') {
                flag = false;
                i = msgRcv.data[1];
            }
            while(j < i) {
                enviarmsg(output, &msgRcv, sizeof(msgRcv));
                j++;
            }
        }
        j = 0;
        i = 1;
    }
}

pid_t startCajero(int* input, int* output) {
    return startMiddleWare(input, output, QCAJEROPATHIN, QCAJEROIDIN, QCAJEROPATHOUT, QCAJEROIDOUT);
}

void enviarPedidoStr(int input, char* pedido, int id) {
    Message msgSend;
    msgSend.mtype = id;
    strncpy(msgSend.data, pedido, 4);
    enviarmsg(input, &msgSend, sizeof(msgSend));
}

void enviarPedidoMsg(int input, Message msg) {
    enviarmsg(input, &msg, sizeof(msg));
}

void recibirPedido(int output, char* pedido, int* idRcv) {
    Message msgRcv;
    recibirmsg(output, &msgRcv, sizeof(msgRcv));
    strncpy(pedido, msgRcv.data, 4);
    *idRcv = msgRcv.mtype;
}