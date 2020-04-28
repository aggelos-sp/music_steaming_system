#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
typedef struct queue_node {
  int songID; 
  struct queue_node *next;
}Q_NODE;


typedef struct unbounded_queue {
    Q_NODE* head;
    Q_NODE* tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;
}QUEUE;

QUEUE** my_queues;

void init_queues(int M);
void enqueue(int songID, int select);
int dequeue(int select);
Q_NODE* create_queue_node(int songID);