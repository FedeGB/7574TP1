//
// Created by fedenote on 4/2/17.
//

#include "Middleware.h"

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
                    std::string number = std::to_string(msgRcv.mtype);
                    appendString(msgRcv.data, number.c_str(), buffer, 10);
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
//                    close(input);
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
