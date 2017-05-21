//
// Created by fedenote on 3/24/17.
//

#include "Utilities.h"


void esperarAleatorio() {
    int sleepingTime = numeroAleatorioRango(3, 7); // Entre 1 y 4 segundo
    sleep(sleepingTime);
}

int numeroAleatorioRango(int min, int max) {
    int random = rand();
    return ((random % (max - min + 1)) + min);
}


const char* appendString(const char* stringOriginal, const char* append, int finalSize){
    std::string newString(stringOriginal);
    newString.insert(4, append);
    while(newString.size() < finalSize) {
        newString.insert(newString.size() - 1, ".");
    }
    return newString.c_str();
}

int countDigits(long number) {
    int count = 0;
    while(number != 0)
    {
        // n = n/10
        number /= 10;
        ++count;
    }
    return count;
}