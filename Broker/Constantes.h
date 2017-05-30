//
// Created by fedenote on 5/26/17.
//

#ifndef INC_7574TP1_CONSTANTES_H
#define INC_7574TP1_CONSTANTES_H


typedef struct entity {
    char type;
    long id;
    long fishPid;
    int weight;
};


#define QINCOMINGID 1
#define QINCOMINGPATH "/bin/grep"
#define QOUTGOINGID 2
#define QOUTGOINGPATH "/bin/grep"


#define SHAREDTABLEID 1
#define SHAREDTABLEPATH "/bin/gunzip"
#define SHAREDCANTID 3
#define SHAREDCANTPATH "/bin/gunzip"
#define SEMTABLEID 2
#define SEMTABLEPATH "/bin/gunzip"

#define BROKERIP "127.0.0.1"
#define BROKERPORT 8085

#endif //INC_7574TP1_CONSTANTES_H
