//
// Created by fedenote on 5/23/17.
//

#include <stdio.h>
#include "registerer.h"
#include <unistd.h>
#include <string>
#include <fstream>

long* getid_2_svc(void* msg, struct svc_req* req) {
    static long result;
    std::ifstream infile ("registro",std::ifstream::binary);

    infile.seekg (0,infile.end);
    long size = infile.tellg();
    infile.seekg (0);

    char* buffer = new char[size];

    infile.read (buffer,size);
    std::string number(buffer);
    long num = std::stol(number);
    result = num;
    num += 1;
    std::string numStore = std::to_string(num);
    infile.close();
    std::ofstream outfile ("registro",std::ofstream::binary);
    outfile.write (numStore.c_str(), numStore.size());

    delete[] buffer;
    outfile.close();
    return (&result);
}