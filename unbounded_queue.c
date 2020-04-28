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

void enqueue(int songID, int select){
    Q_NODE* new_node = create_queue_node(songID);
    pthread_mutex_lock(&my_queues[select]->tail_lock);
    my_queues[select]->tail->next = new_node;
    my_queues[select]->tail = new_node;
    pthread_mutex_unlock(&my_queues[select]->tail_lock);
}

int dequeue(int select){
    int to_return = -2;

    pthread_mutex_lock(&my_queues[select]->head_lock);

    if(my_queues[select]->head->next == NULL){
        printf(ANSI_COLOR_RED"Trying to dequeue from empty queue.\n"ANSI_COLOR_RESET);
    }else{
        to_return = my_queues[select]->head->next->songID;
        my_queues[select]->head = my_queues[select]->head->next;
    }

    pthread_mutex_unlock(&my_queues[select]->head_lock);
    return to_return;
}