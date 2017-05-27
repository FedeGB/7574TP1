//
// Created by fedenote on 5/26/17.
//

#include "Broker.h"



pid_t startBroker() {
    pid_t broker = fork();
    if(broker == 0) {


        return 0;
    } else {
        return broker;
    }
}