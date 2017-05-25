//
// Created by fedenote on 4/10/17.
//

#include "HeladeroMOM.h"


pid_t startHeladeroMOM(int* queues) {
    pid_t middle = fork();
    if(middle == 0) {
        int regisInput, regisOut;
        regisInput = getmsg(QREGISTROHELINID, QREGISTROHELINPATH);
        regisOut = getmsg(QREGISTROHELOUTID, QREGISTROHELOUTPATH);
        pid_t registering = registerer(regisInput, regisOut);
        if(registerer == 0) {
            return 0;
        }
        std::vector<pid_t> trabajadores;
        pid_t working;
        struct sockaddr_in clientAddr;
        socklen_t longitudCliente = sizeof(clientAddr);
        printf("Estoy esperando a recibir conexion...\n");
        int newCjfd = receiveConnection(queues[0], (struct sockaddr*)&clientAddr, &longitudCliente);
        if(newCjfd > 0) {
            printf("Recibi nueva conexión\n");
        } else {
            perror("Fallo en recibir nueva conexión");
        }
        int clientePort = 8081;
        const char* clienteIP = "127.0.0.1";
        int toClfd = connectTo(queues[3], clientePort, clienteIP);
        if(toClfd < 0) {
            printf("No se pudo conectar a %s con puerto %d\n", clienteIP, clientePort);
        } else {
            printf("Me conecte a %s con puerto %d\n", clienteIP, clientePort);
        }
        int fromCj = getmsg(QFROMCAJEROHELID, QFROMCAJEROHELPATH);
        int toCl = getmsg(QTOCLIENTEHELID, QTOCLIENTEHELPATH);
        working = work(newCjfd, fromCj, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toCl, queues[3], true);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        waitpid(registering, NULL, 0);
        for(std::vector<pid_t>::iterator it = trabajadores.begin(); it != trabajadores.end(); it++) {
            waitpid(*it, NULL, 0);
        }
        return 0;
    } else {
        return middle;
    }
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