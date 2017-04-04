//
// Created by fedenote on 4/2/17.
//

#include "Middleware.h"


pid_t startMiddleWare(int regIn, char* pathRegIn, int regOut, char* pathRegOut,
                      char* pathIn, int varIn, char* pathOut, int varOut) {
    pid_t middle = fork();
    if(middle == 0) {
        int registroIn = creamsg(regIn, pathRegIn);
        int registroOut = creamsg(regOut, pathRegOut);
        int input = creamsg(varIn, pathIn);
        int output = creamsg(varOut, pathOut);
        pid_t registerer = registerer(registroIn, registroOut);
        if(registerer == 0) {
            return 0;
        }
        pid_t working = work(input, output);
        if(working == 0) {
            return 0;
        }
        waitpid(registerer, NULL, 0);
        waitpid(working, NULL, 0);

        elimsg(registroIn);
        elimsg(registroOut);
        elimsg(input);
        elimsg(output);
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