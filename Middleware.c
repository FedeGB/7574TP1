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
        std::vector<qPedido> colasDeMiddleware;
        pid_t working;
        bool sendSocket;
        for(int i = 0; i < qCantidad; i++) {
//            if(queues[i].isSocket) {
//                qGetter = queues[i].qId;
//            } else {
//                qGetter = getmsg(queues[i].qId, queues[i].qPath);
//            }
            sendSocket = false;
            if(i%2 != 0 && i != 0) {
                colasDeMiddleware.push_back(queues[i]);
                if(queues[i].isSocket) {
                    sendSocket = true;
                    int conn = connectTo(colasDeMiddleware[i].qId, colasDeMiddleware[i].port, colasDeMiddleware[i].ip);
                    if(conn < 0) {
                        printf("No se pudo conectar a %s con puerto %d\n", colasDeMiddleware[i].ip, colasDeMiddleware[i].port);
                    }
                }
                working = work(colasDeMiddleware[i-1].qId, colasDeMiddleware[i].qId, sendSocket);
                if(working == 0) {
                    return 0;
                }
                trabajadores.push_back(working);
            } else {
                if(queues[i].isSocket) {
                    struct sockaddr clientAddr;
                    unsigned int longitudCliente;
                    printf("Estoy esperando a recibir conexion...\n");
                    int newSfd = receiveConnection(queues[i].qId, (struct sockaddr*)&clientAddr, &longitudCliente);
                    if(newSfd > 0) {
                        queues[i].qId = newSfd;
                        printf("Recibi nueva conexión\n");
                    } else {
                        printf("Fallo en recibir nueva conexión\n");
                    }
                }
                colasDeMiddleware.push_back(queues[i]);
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


pid_t work(int input, int output, bool sendSocket) {
    pid_t trabajo = fork();
    if(trabajo == 0) {
        int status;
        int sndSts;
        while (true) {
            Message msgRcv;
            if(sendSocket) {
                status = recibirmsg(input, &msgRcv, sizeof(msgRcv), 0);
            } else {
                status = receiveFrom(input, &msgRcv); // Socket
            }
            if (status >= 0) {
                if(sendSocket) {
                    char buffer[10];
                    char number[countDigits(msgRcv.mtype)];
                    sprintf(number, "%ld", msgRcv.mtype);
                    strncpy(buffer, appendString(msgRcv.data, number, 10), 10);
                    sndSts = sendTo(output, msgRcv.data, sizeof(msgRcv.data)); // Socket
                    if(sndSts < 0){
                        return 0;
                    }
                } else {
                    enviarmsg(output, &msgRcv, sizeof(msgRcv));
                }
            } else {
                if(!sendSocket) {
                    close(input);
                }
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
                msgSnd.mtype = msgRcv.mtype;
                int memReg = getshm(REGISTERHANDLERID, REGISTERHANDLERPATH);
                if(memReg < 0) {
                    return 0;
                }
                p(registerSem);
                long* id = (long*)map(memReg);
                (*id)++;
                const int n = snprintf(NULL, 0, "%lu", *id);
                char buf[n+1];
                snprintf(buf, n+1, "%lu", *id);
                strcpy(msgSnd.data, buf);
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