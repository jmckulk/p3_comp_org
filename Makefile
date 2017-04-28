CC = g++
CFLAGS = -g -Wall -std=c++11
TARGET = main

all: main

main: main.o MSI.o
	$(CC) $(CFLAGS) -o main main.o MSI.o

main.o: main.cpp MSI.h
	$(CC) $(CFLAGS) -c main.cpp

MSI.o: MSI.cpp MSI.h
	$(CC) $(CFLAGS) -c MSI.cpp

clean:
	$(RM) main *.o *~
