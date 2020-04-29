#include "optimistic_list.h"

L_NODE* create_list_node(int songID){
    L_NODE* node = malloc(sizeof(L_NODE));
    if(node == NULL){
        printf(ANSI_COLOR_RED"Failed to allocate memory for list.\n"ANSI_COLOR_RESET);
        exit(0);
    }
    node->songID = songID;
    node->next = NULL;
    pthread_mutex_init(&node->lock);
}

int validate(L_NODE* pred, L_NODE* curr){
    L_NODE* tmp = my_list->head;
    while(tmp->songID <= pred->songID){
        if(tmp == pred){
            if(pred->next == curr){
                return true;
            }else{
                return false;
            }
        }
        tmp = tmp->next;
    }
    return false;
}

int insert_list(int songID){
    L_NODE* pred;
    L_NODE* curr;
    L_NODE* new_node;
    int result;
    int return_flag = false;
    if(my_list->head == NULL){
        pthread_mutex_lock(&list_lock);
        my_list->head = create_list_node(songID);
        pthread_mutex_unlock(&list_lock);
    }
    while(true){
        pred = my_list->head;
        curr = pred->next;
        while(curr->songID < songID){
            pred = curr;
            curr = curr->next;
        }
        pthread_mutex_lock(&pred->lock);
        pthread_mutex_lock(&curr->lock);
        if(validate(pred, curr)){
            if(songID == curr->songID){
                result = false;
                return_flag = true;
            }else{
                new_node = create_list_node(songID);
                new_node->next = curr;
                pred->next = new_node;
                result = true;
                return_flag = true;
            }
        }
        pthread_mutex_unlock(&pred->lock);
        pthread_mutex_unlock(&curr->lock);
        if(return_flag){
            return result;
        }
    }
}

void init_list(){
    my_list->head = NULL;
    my_list->tail = NULL;
    pthread_mutex_init(&list_lock, NULL);
}