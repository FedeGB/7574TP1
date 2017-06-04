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

// Queues
#define QINCOMINGID 1
#define QINCOMINGPATH "/bin/grep"
#define QOUTGOINGID 2
#define QOUTGOINGPATH "/bin/grep"
#define QINGUSTOSID 3
#define QINGUSTOSPATH "/bin/grep"
#define QOUTGUSTOSID 4
#define QOUTGUSTOSPATH "/bin/grep"
#define QINHELADERIAID 5
#define QINHELADERIAPATH "/bin/grep"
#define QOUTHELADERIAID 6
#define QOUTHELADERIAPATH "/bin/grep"

// Shared Memory
#define SHAREDTABLEID 1
#define SHAREDTABLEPATH "/bin/gunzip"
#define SHAREDCANTID 3
#define SHAREDCANTPATH "/bin/gunzip"
#define SEMTABLEID 2
#define SEMTABLEPATH "/bin/gunzip"

#define ENTRADAPATH "/bin/dir"
#define ENTRADAID 1
#define LUGARESPATH "/bin/dir"
#define LUGARESID 2
#define LUGARESCAJEROPATH "/bin/dir"
#define LUGARESCAJEROID 3

// Semaforos
#define SEMLUGARESPATH "/bin/sh"
#define SEMLUGARESID 2
#define SEMLUGARESCAJPATH "/bin/sh"
#define SEMLUGARESCAJID 3
#define SEMENTRADAPATH "/bin/sh"
#define SEMENTRADAID 4

#define SEMGUSTOS "/bin/ls"

// Gustos
#define VAINILLA '1'
#define FRUTILLA '2'
#define DULCEDELECHE '3'
#define LIMON '4'
#define SAMBAYON '5'
#define CREMAAMERICANA '6'
#define MENTAGRANIZADA '7'

// Variables heladeria
#define ESPACIOHELADERIA 10
#define MAXCOLACAJER 4

#define BROKERIP "127.0.0.1"
#define BROKERPORT 8085

#endif //INC_7574TP1_CONSTANTES_H
