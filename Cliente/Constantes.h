//
// Created by fedenote on 3/18/17.
//

#ifndef INC_7574TP1_CONSTANTES_H
#define INC_7574TP1_CONSTANTES_H

// IPCS
// Queues
#define QCAJEROTOCLPATH "/bin/sh"
#define QCAJEROTOCLID  2
#define QHELADEROTOCLPATH "/bin/sh"
#define QHELADEROTOCLID 9
#define QTOCAJEROCLPATH "/bin/sh"
#define QTOCAJEROCLID 10
#define QFROMCAJEROCLPATH "/bin/sh"
#define QFROMCAJEROCLID 11

#define QREGISTROCLINPATH "/bin/echo"
#define QREGISTROCLINID 5
#define QREGISTROCLOUTPATH "/bin/echo"
#define QREGISTROCLOUTID 6

// Semaforos
#define SEMLUGARESPATH "/bin/sh"
#define SEMLUGARESID 2
#define SEMLUGARESCAJPATH "/bin/sh"
#define SEMLUGARESCAJID 3
#define SEMENTRADAPATH "/bin/sh"
#define SEMENTRADAID 4

// Shared Memory
#define ENTRADAPATH "/bin/dir"
#define ENTRADAID 1
#define LUGARESPATH "/bin/dir"
#define LUGARESID 2
#define LUGARESCAJEROPATH "/bin/dir"
#define LUGARESCAJEROID 3

// TIPO ÞEDIDO
#define LLEVAR '1'
#define ENLUGAR '2'

// Variables heladeria
#define ESPACIOHELADERIA 10
#define MAXCOLACAJER 4

// Socket
//#define CLIENTEIP "127.0.0.1"
//#define CLIENTEPORT 8081
#define BROKERIP "127.0.0.1"
#define BROKERPORT 8085

#endif //INC_7574TP1_CONSTANTES_H
