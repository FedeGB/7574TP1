//
// Created by fedenote on 4/16/17.
//

#include "ClienteMOM.h"


pid_t startClienteMOM(int* queues) {
    /*qPedido colasMiddle[6];
    qPedido registro[2];
//    colasMiddle[0].qId = QCAJEROTOCLID;
//    colasMiddle[0].qPath = QCAJEROTOCLPATH;
    colasMiddle[0].qId = queues[1];
    colasMiddle[0].isSocket = true;
    colasMiddle[0].doReceive = true;
    colasMiddle[1].qId = QFROMCAJEROCLID;
    colasMiddle[1].qPath = QFROMCAJEROCLPATH;
    colasMiddle[1].isSocket = false;
    colasMiddle[2].qId = QTOCAJEROCLID;
    colasMiddle[2].qPath = QTOCAJEROCLPATH;
    colasMiddle[2].isSocket = false;
//    colasMiddle[3].qId = QCLIENTETOCJID;
//    colasMiddle[3].qPath = QCLIENTETOCJPATH;
    colasMiddle[3].qId = queues[2];
    colasMiddle[3].isSocket = true;
    colasMiddle[3].doConnect = false;
    colasMiddle[3].ip = "127.0.0.1";
    colasMiddle[3].port = 8082;
//    colasMiddle[4].qId = QHELADEROTOCLID;
//    colasMiddle[4].qPath = QHELADEROTOCLPATH;
    colasMiddle[4].qId = queues[10];
    colasMiddle[4].isSocket = true;
    colasMiddle[4].doReceive = true;
    colasMiddle[5].qId = QFROMHELADEROCLID;
    colasMiddle[5].qPath = QFROMHELADEROCLPATH;
    colasMiddle[5].isSocket = false;
    registro[0].qId = QREGISTROCLINID;
    registro[0].qPath = QREGISTROCLINPATH;
    registro[1].qId = QREGISTROCLOUTID;
    registro[1].qPath = QREGISTROCLOUTPATH;
    return startMiddleWare(colasMiddle, 6, registro);*/
    pid_t middle = fork();
    if(middle == 0) {
        int regisInput, regisOut;
        regisInput = getmsg(QREGISTROCLINID, QREGISTROCLINPATH);
        regisOut = getmsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
        pid_t registering = registerer(regisInput, regisOut);
        if(registerer == 0) {
            return 0;
        }
        std::vector<pid_t> trabajadores;
        pid_t working;
        struct sockaddr_in clientAddr;
        socklen_t longitudCliente = sizeof(clientAddr);
        printf("Estoy esperando a recibir conexion...\n");
        int newCjfd = receiveConnection(queues[1], (struct sockaddr*)&clientAddr, &longitudCliente);
        if(newCjfd > 0) {
            printf("Recibi nueva conexión\n");
        } else {
            perror("Fallo en recibir nueva conexión");
        }
        struct sockaddr_in clientAddr2;
        socklen_t longitudCliente2 = sizeof(clientAddr2);
        printf("Estoy esperando a recibir conexion...\n");
        int newHlfd = receiveConnection(queues[10],  (struct sockaddr*)&clientAddr2, &longitudCliente2);
        if(newHlfd > 0) {
            printf("Recibi nueva conexión\n");
        } else {
            perror("Fallo en recibir nueva conexión");
        }
        int fromCj = getmsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
        int toCj = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
        int fromHl = getmsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
        working = work(newCjfd, fromCj, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toCj, newCjfd, true);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(newHlfd, fromHl, false);
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