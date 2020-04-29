CC=gcc
CFLAGS=-Wall -ansi -pedantic -pthread -std=gnu99
DEPS= fine_grained_bst.h unbounded_queue.h optimistic_list.h
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
all: main.o fine_grained_bst.o unbounded_queue.o optimistic_list.o
	$(CC) $(CFLAGS) main.c fine_grained_bst.c unbounded_queue.c optimistic_list.c

clean:
	rm -rf a.out *o
