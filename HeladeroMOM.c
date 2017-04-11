//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroCajeroMOM() {
    return startMiddleWare(QREGISTROHELCAJINID, QREGISTROHELCAJINPATH, QREGISTROHELCAJOUTID,
                           QREGISTROHELCAJOUTPATH, QHELCAJPATHIN, QHELCAJIDIN, QHELCAJPATHOUT, QHELCAJIDOUT);
}

void devolverPedidoCliente(char* pedido, long idCliente) {

}

void recibirPedidoCajero(char* pedido, long* idCliente) {

}

void recibirPedidoHeladero(char* pedido, long idCliente) {

}