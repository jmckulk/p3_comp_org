CC = g++
CFLAGS = -g -Wall -std=c++11
TARGET = main

all: main

main: main.o
	$(CC) $(CFLAGS) -o main main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp


clean:
	$(RM) main *.o *~
