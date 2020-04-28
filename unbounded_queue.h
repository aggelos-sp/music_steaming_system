#include <pthread.h>
#include <stdlib.h>

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

QUEUE *my_queues;

void init_queues(int M);
void enqueue(int songID, int select);
int dequeue(int select);