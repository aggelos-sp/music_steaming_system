CC=gcc
CFLAGS=-Wall -ansi -pedantic -pthread
DEPS= fine_grained_bst.h
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
all: main.o fine_grained_bst.o
	$(CC) $(CFLAGS) main.c fine_grained_bst.c 

clean:
    rm -rf run
