//
// Created by fedenote on 4/16/17.
//

#include "ClienteMOM.h"


pid_t startClienteMOM(int* queues) {
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
        int newCjfd = connectTo(queues[0], BROKERPORT, BROKERIP);
        if(newCjfd >= 0) {
            printf("Me conecte a %s en el puerto %i\n", BROKERIP, BROKERPORT);
        } else {
            printf("No me pude conectar a %s en el puerto %i\n", BROKERIP, BROKERPORT);
        }
        int fromCj = getmsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
        int toCj = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
        int fromHl = getmsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
        working = work(queues[0], fromCj, false);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(toCj, queues[0], true);
        if(working == 0) {
            return 0;
        }
        trabajadores.push_back(working);
        working = work(queues[0], fromHl, false);
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
    strncpy(msgSend.data, pedido, 5);
    msgSend.data[4] = 'j';
    enviarmsg(queue, &msgSend, sizeof(msgSend));
    return true;
}

bool recibirTicket(char* ticket, long idCliente) {
    int queueAviso = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    if(queueAviso < 0) {
        return false;
    }
    Message msgWar;
    msgWar.mtype = 1;
    strncpy(msgWar.data, "r0000", 5);
    enviarmsg(queueAviso, &msgWar, sizeof(msgWar));
    int queue = getmsg(QFROMCAJEROCLID, QFROMCAJEROCLPATH);
    if(queue < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente) < 0) {
        return false;
    }
    strncpy(ticket, msgRcv.data, 5);
    return true;
}

bool recibirHelado(char* pedido, long idCliente) {
    int queueAviso = getmsg(QTOCAJEROCLID, QTOCAJEROCLPATH);
    if(queueAviso < 0) {
        return false;
    }
    Message msgWar;
    msgWar.mtype = 1;
    strncpy(msgWar.data, "r0000", 5);
    enviarmsg(queueAviso, &msgWar, sizeof(msgWar));
    int queue = getmsg(QFROMHELADEROCLID, QFROMHELADEROCLPATH);
    if(queue < 0) {
        return false;
    }
    Message msgRcv;
    if(recibirmsg(queue, &msgRcv, sizeof(msgRcv), idCliente) < 0) {
        return false;
    }
    strncpy(pedido, msgRcv.data, 5);
    return true;
}

long registrarCliente() {
    int regIn = getmsg(QREGISTROCLINID, QREGISTROCLINPATH);
    int regOut = getmsg(QREGISTROCLOUTID, QREGISTROCLOUTPATH);
    Message regMsg;
    regMsg.mtype = getpid();
    strncpy(regMsg.data, "rrrrr", 5);
    enviarmsg(regIn, &regMsg, sizeof(regMsg));
    Message regRcv;
    if(recibirmsg(regOut, &regRcv, sizeof(regRcv), getpid()) >= 0) {
        return atol(regRcv.data);
    }
    return -1;
}

bool handshake(char* registering, long idRegister) {
    return enviarPedidoCajero(registering, idRegister);
}