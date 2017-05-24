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
                    sndSts = sendTo(output, &msgRcv, 10); // Socket
                    printf("Envie mensaje por socket: %s\n", msgRcv.data);
                    if(sndSts < 0){
                        return 0;
                    }
                } else {
                    enviarmsg(output, &msgRcv, sizeof(msgRcv));
                    printf("Envie mensaje por cola: %s\n", msgRcv.data);
                }
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
                msgSnd.mtype = msgRcv.mtype;
                int memReg = getshm(REGISTERHANDLERID, REGISTERHANDLERPATH);
                if(memReg < 0) {
                    return 0;
                }
                p(registerSem);
                long val = getRegisteringFromRPC();
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


long getRegisteringFromRPC() {
    CLIENT *clnt;
    long *result;
    char *server;
    long message;

    strcpy(server, "127.0.0.1");
    clnt = clnt_create(server, REGISTERERSERV, GETIDVERS, "tcp");
    if (clnt == (CLIENT *) NULL) {
        clnt_pcreateerror(server);
        return -1;
    }
    result = getid_1(NULL, clnt);
    if (result == (long *) NULL) {
        clnt_perror(clnt, server);
        return -1;
    }
    if (*result == 0) {
        fprintf(stderr, "Register: could get id\n");
        return -1;
    }
    printf("ID obtenido es: %ld\n", *result);
    clnt_destroy(clnt);
    return *result;
}