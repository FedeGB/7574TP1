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
                if(working == 0) {
                    return 0;
                }
                trabajadores.push_back(working);
            }
        }
        waitpid(registering, NULL, 0);
        for(std::vector<pid_t>::iterator it = trabajadores.begin(); it != trabajadores.end(); it++) {
            waitpid(*it, NULL, 0);
        }
        return 0;
    } else {
        return middle;
    }
}


pid_t work(int input, int output) {
    pid_t trabajo = fork();
    if(trabajo == 0) {
        int status;
        while (true) {
            Message msgRcv;
            status = recibirmsg(input, &msgRcv, sizeof(msgRcv), 0);
            if (status >= 0) {
                enviarmsg(output, &msgRcv, sizeof(msgRcv));
            } else {
                return 0;
            }
        }
    } else {
        return trabajo;
    }
}

pid_t registerer(int registroIn, int registroOut) {
    pid_t reg = fork();
    int registerSem = getSemaforo(SEMREGISTERID, SEMREGISTERPATH);
    if(reg == 0) {
        int status;
        while (true) {
            Message msgRcv;
            status = recibirmsg(registroIn, &msgRcv, sizeof(msgRcv), 0);
            if (status >= 0) {
                Message msgSnd;
                strncpy(msgSnd.data, "1111", 4);
                int memReg = getshm(REGISTERHANDLERID, REGISTERHANDLERPATH);
                if(memReg < 0) {
                    return 0;
                }
                p(registerSem);
                long* id = (long*)map(memReg);
                *id++;
                msgSnd.mtype = *id;
                unmap(id);
                v(registerSem);
                enviarmsg(registroOut, &msgSnd, sizeof(msgSnd));
            } else {
                return 0;
            }
        }
    } else {
        return reg;
    }
}