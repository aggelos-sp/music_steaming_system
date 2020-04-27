CC=gcc
CFLAGS= -Wall -ansi -pedantic -pthread

all: main.c fine_grained_bst.c
        $(CC) $(CFLAGS) main.c fine_grained_bst.c 

clean:
        rm -rf run
