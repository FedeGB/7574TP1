//
// Created by fedenote on 5/23/17.
//

#include <stdio.h>
#include "registerer.h"
#include <unistd.h>

long* getid_1_svc(void* msg, struct svc_req* req) {
    static long result;
    FILE* f = NULL;
    if( access( "registering", F_OK ) == -1 ) {
        // file doesn't exist
        f = fopen("registering", "a+");
        if (f == (FILE *) NULL) {
            result = 0;
            return (&result);
        }
        long* firstNumber;
        *firstNumber = 1;
        int status;
        status = fprintf(f, "%ld", *firstNumber);
        if(status < 0) {
            perror("No pudo escribir registro");
            result = 0;
            return (&result);
        }
        result = *firstNumber;
        fclose(f);
    } else {
        f = fopen("registering", "r+");
        if (f == (FILE *) NULL) {
            result = 0;
            return (&result);
        }
        long* readNumber;
        fscanf(f, "%ld", readNumber);
        *readNumber +=1;
        result = *readNumber;
        fprintf(f, "%ld", *readNumber);
        fclose(f);
    }
    return (&result);
}