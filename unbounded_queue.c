#include "unbounded_queue.h"


Q_NODE* create_queue_node(int songID){
    Q_NODE* node;
    node = malloc(sizeof(Q_NODE));
    if(node == NULL){
        printf(ANSI_COLOR_RED"Couldn't allocate memory for queue node.\n"ANSI_COLOR_RESET);
        exit(0);
    }
    node->songID = songID;
    node->next = NULL;
    return node;
}
void init_queues(int M){
    Q_NODE* sentinel;
    int i = 0;
    my_queues = malloc(M * sizeof(QUEUE));
    if(my_queues == NULL){
        printf(ANSI_COLOR_RED"Couldn't allocate memory for queues.\n"ANSI_COLOR_RESET);
        exit(0);
    }
    for(i = 0; i < M; i++){
        sentinel = create_queue_node(-1);
        my_queues[i]->head = sentinel;
        my_queues[i]->tail = sentinel;
        pthread_mutex_init(&my_queues[i]->head_lock, NULL);
        pthread_mutex_init(&my_queues[i]->tail_lock, NULL);
    }
}