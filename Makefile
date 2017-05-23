CC=g++
CFLAGS=-c -Wall -ggdb
LDFLAGS=-std=c++11
SOURCES=*.c IPC/*.c Common/*.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


