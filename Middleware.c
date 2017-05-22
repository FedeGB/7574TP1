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
                if(queues[i].isSocket) {
                    sendSocket = true;
                    if(queues[i].doConnect) {
                        int conn = connectTo(queues[i].qId, queues[i].port, queues[i].ip);
                        if(conn < 0) {
                            printf("No se pudo conectar a %s con puerto %d\n", queues[i].ip, queues[i].port);
                        }
                        printf("Me conecte a %s con puerto %d\n", queues[i].ip, queues[i].port);
                    }
                } else {
                    queues[i].qId = getmsg(queues[i].qId, queues[i].qPath);
                }
                colasDeMiddleware.push_back(queues[i]);
                working = work(colasDeMiddleware[i-1].qId, colasDeMiddleware[i].qId, sendSocket);
                if(working == 0) {
                    return 0;
                }
                trabajadores.push_back(working);
            } else {
                if(queues[i].isSocket && queues[i].doReceive) {
                    struct sockaddr_in clientAddr;
                    socklen_t longitudCliente = sizeof(clientAddr);
                    printf("Estoy esperando a recibir conexion...\n");
                    int newSfd = receiveConnection(queues[i].qId, (struct sockaddr*)&clientAddr, &longitudCliente);
                    if(newSfd > 0) {
                        queues[i].qId = newSfd;
                        printf("Recibi nueva conexión\n");
                    } else {
                        perror("Fallo en recibir nueva conexión");
                    }
                } else if(!queues[i].isSocket) {
                    queues[i].qId = getmsg(queues[i].qId, queues[i].qPath);
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
                printf("Recibi mensaje de cola: %s\n", msgRcv.data);
            } else {
                status = receiveFrom(input, &msgRcv); // Socket
                printf("Recibi mensaje de socket: %s\n", msgRcv.data);
            }
            if (status >= 0) {
                if(sendSocket) {
                    char buffer[10];
                    char number[countDigits(msgRcv.mtype)];
                    appendString(msgRcv.data, number, buffer, 10);
                    printf("Envio mensaje por socket: %s\n", buffer);
                    sndSts = sendTo(output, buffer, sizeof(buffer)); // Socket
                    printf("Envie mensaje por socket: %s\n", buffer);
                    if(sndSts < 0){
                        return 0;
                    }
                } else {
                    enviarmsg(output, &msgRcv, sizeof(msgRcv));
                    printf("Envie mensaje por cola: %s\n", msgRcv.data);
                }
            } else {
                if(!sendSocket) {
                    close(input);
                }
                return 0;
            }
        }
    } else {
        /*if(sendSocket) {
            close(output);
        } else {
            close(input);
        }*/
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
