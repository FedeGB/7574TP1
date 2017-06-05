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
            } else {
                status = receiveFrom(input, &msgRcv); // Socket
            }
            if (status >= 0) {
                if(sendSocket) {
                    sndSts = sendTo(output, &msgRcv, 10); // Socket
                    if(sndSts < 0){
                        return 0;
                    }
                } else {
                    enviarmsg(output, &msgRcv, sizeof(msgRcv));
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
    if(reg == 0) {
        int status;
        while (true) {
            Message msgRcv;
            status = recibirmsg(registroIn, &msgRcv, sizeof(msgRcv), 0);
            if (status >= 0) {
                Message msgSnd;
                msgSnd.mtype = msgRcv.mtype;
                long id = getRegisteringFromRPC();
                printf("Recibi id de RPC: %ld\n", id);
                const int n = snprintf(NULL, 0, "%lu", id);
                char buf[n+1];
                snprintf(buf, n+1, "%lu", id);
                strncpy(msgSnd.data, buf, 5);
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
    char server[50];
    long message;

    strcpy(server, RPCSERVERIP);
    clnt = clnt_create(server, REGISTERERSERV, GETIDVERS, "tcp");
    if (clnt == (CLIENT *) NULL) {
        clnt_pcreateerror(server);
        return -1;
    }
    result = getid_2(NULL, clnt);
    if (result == (long *) NULL) {
        clnt_perror(clnt, server);
        return -1;
    }
    if (*result == 0) {
        fprintf(stderr, "Register: could get id\n");
        return -1;
    }
    clnt_destroy(clnt);
    return *result;
}