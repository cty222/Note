CC = gcc
INCLUDE = .

all: new1 clean

new1: Main otherFunction
	$(CC) -o new1 Main.o wavFunction.o playFunction.o -lasound

Main: 
	$(CC) -c Main.c

otherFunction: 
	$(CC) -c wavFunction.c
	$(CC) -c playFunction.c

clean:
	rm *.o
