#include "unbounded_queue.h"


void init_queues(int M){
    my_queues = malloc(M * sizeof(QUEUE));
    if(my_queues == NULL){
        printf(ANSI_COLOR_RED"Couldn't allocate memory for queues.\n"ANSI_COLOR_RESET);
    }
}