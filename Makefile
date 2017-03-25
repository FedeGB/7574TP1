CC=g++
CFLAGS=-c -Wall -ggdb
LDFLAGS=
SOURCES=main.c MemoriaCompartida.c ColaMensajes.c Semaforo.c Cajero.c Heladero.c Cliente.c Utilities.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


