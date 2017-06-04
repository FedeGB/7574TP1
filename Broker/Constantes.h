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
} entity;


#define QINCOMINGID 1
#define QINCOMINGPATH "/bin/grep"
#define QOUTGOINGID 2
#define QOUTGOINGPATH "/bin/grep"
#define QINGUSTOSID 3
#define QINGUSTOSPATH "/bin/grep"
#define QOUTGUSTOSID 4
#define QOUTGUSTOSPATH "/bin/grep"

#define SHAREDTABLEID 1
#define SHAREDTABLEPATH "/bin/gunzip"
#define SHAREDCANTID 3
#define SHAREDCANTPATH "/bin/gunzip"
#define SEMTABLEID 2
#define SEMTABLEPATH "/bin/gunzip"

// Semaforos
#define SEMGUSTOS "/bin/ls"

// Gustos
#define VAINILLA '1'
#define FRUTILLA '2'
#define DULCEDELECHE '3'
#define LIMON '4'
#define SAMBAYON '5'
#define CREMAAMERICANA '6'
#define MENTAGRANIZADA '7'

#define BROKERIP "127.0.0.1"
#define BROKERPORT 8085

#endif //INC_7574TP1_CONSTANTES_H
