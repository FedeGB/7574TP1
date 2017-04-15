//
// Created by fedenote on 4/2/17.
//

#include "Middleware.h"


pid_t startMiddleWare(qPedido* queues, int qCantidad, qPedido* regQueues) {
    pid_t middle = fork();
    int qGetter;
    if(middle == 0) {
        int regis;
        std::vector<int> qReg;
        for(int reg = 0; reg < 2; reg++) {
            regis = getmsg(regQueues[reg].qId, regQueues[reg].qPath);
            qReg.push_back(regis);
        }
        pid_t registering = registerer(qReg[0], qReg[1]);
        if(registerer == 0) {
            return 0;
        }
        std::vector<pid_t> trabajadores;
        std::vector<int> colasDeMiddleware;
        pid_t working;
        for(int i = 0; i < qCantidad; i++) {
            qGetter = getmsg(queues[i].qId, queues[i].qPath);
            colasDeMiddleware.push_back(qGetter);
            if(i%2 != 0 && i != 0) {
                working = work(colasDeMiddleware[i-1], colasDeMiddleware[i]);
                trabajadores.push_back(working);
                if(working == 0) {
                    return 0;
                }
            }
        }
        waitpid(registering, NULL, 0);
        for(std::vector<pid_t>::iterator it = trabajadores.begin(); it != trabajadores.end(); it++) {
            waitpid(*it, NULL, 0);
        }
//        elimsg(qReg[0]);
//        elimsg(qReg[1]);
//        for(std::vector<int>::iterator it2 = colasDeMiddleware.begin(); it2 != colasDeMiddleware.end(); it2++) {
//            elimsg(*it2);
//        }
        return 0;
    } else {
        return middle;
    }
}


pid_t work(int input, int output) {
    pid_t trabajo = fork();
    if(trabajo == 0) {
        bool flag = true;
        int status;
        int j = 0;
        int i = 1;
        while (flag) {
            Message msgRcv;
            status = recibirmsg(input, &msgRcv, sizeof(msgRcv), 0);
            if (status >= 0) {
                if (msgRcv.data[1] == '0') {
                    flag = false;
                    i = msgRcv.data[2];
                }
                while (j < i) {
                    enviarmsg(output, &msgRcv, sizeof(msgRcv));
                    j++;
                }
            }
            j = 0;
            i = 1;
        }
        return 0;
    } else {
        return trabajo;
    }
}

pid_t registerer(int registroIn, int registroOut) {
    pid_t reg = fork();
    if(reg == 0) {
        bool flag = true;
        int status;
        while (flag) {
            Message msgRcv;
            status = recibirmsg(registroIn, &msgRcv, sizeof(msgRcv), 0);
            if (status >= 0) {
                if (msgRcv.data[1] == '0') {
                    flag = false;
                }
                Message msgSnd;
                // TODO: Devolver handler correcto
                strncpy(msgSnd.data, "1111", 4);
                msgSnd.mtype = 1;
                enviarmsg(registroOut, &msgSnd, sizeof(msgSnd));
            }
        }
    } else {
        return reg;
    }
}

int registrar() {

}

//desregistrar


//Pasar handler y no ID de queue

//registrar -> handler
