CC=g++
CFLAGS=-c -Wall -ggdb
LDFLAGS=-std=c++11 -lnsl
SOURCES=*.c ../IPC/*.c ../Common/*.c ../RPCServer/registerer_clnt.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=broker

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


