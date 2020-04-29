#include "fine_grained_bst.h"
#include "unbounded_queue.h"
#include "optimistic_list.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN     "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int number_of_threads = 0;
int bst_node_number = 0;
int bst_checksum = 0;
pthread_barrier_t first_barrier;
pthread_barrier_t second_barrier;
pthread_barrier_t third_barrier;

int debug = false;

void* first(void *id){
    int i = 0;
    int song_id = 0;
    for(i = 0; i < number_of_threads; i++){
        song_id = ((i * number_of_threads) + (int)id);
        if(debug){
            printf("I am thread: %d and going to insert:%d\n",(int)id,song_id);
        }
        insert(song_id,global_root,NULL);
    }
    pthread_barrier_wait(&first_barrier);
    return NULL;
}
void tree_counter(T_NODE* node) 
{ 
    if (node == NULL) 
        return;
    tree_counter(node->lc); 
    //printf("Tree value : (%d)\n", node->songID);
    bst_node_number++;
    bst_checksum += node->songID; 

    tree_counter(node->rc); 
} 
void* first_check(void * arg){
    int N = number_of_threads;
    int correct_checksum = ((N*N)*(N-1)*(N+1))/2;
    tree_counter(global_root);
    if(bst_node_number != (N * N)){
        printf(ANSI_COLOR_RED"total size check failed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET, (N*N), bst_node_number);
    }else{
        printf(ANSI_COLOR_GREEN"total size check passed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET, (N*N), bst_node_number);
    }
    if(bst_checksum != correct_checksum){
        printf(ANSI_COLOR_RED"total keysum check failed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET,correct_checksum,bst_checksum);
    }else{
        printf(ANSI_COLOR_GREEN"total keysum check passed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET,correct_checksum,bst_checksum);
    }
    return NULL;
}

void* second(void* arg){
    int N = number_of_threads;
    int id = (int) arg;
    int i = 0;
    int r = -10;
    int index = 1;
    int select = 0;
    for(i = (N*id); i <= ((N*id) + (N - 1)); i++){
        if(debug){
            printf("I am thread %d and starting to search (%d)..\n",id, i);
        }
        r = search(i,global_root,global_root->p);
        select = (id + index)%(N/2);
        enqueue(r,select);
        index += 1;
    }
    pthread_barrier_wait(&second_barrier);
    return NULL;
}

void* second_check(void* arg){
    int i = 0;
    Q_NODE* tmp = NULL;
    int nodes_found = 0;
    int checksum = 0;
    int N = number_of_threads;
    int correct_checksum =  ((N*N)*(N-1)*(N+1))/2;
    for(i = 0; i < number_of_threads/2; i++){
        tmp = my_queues[i]->head->next;
        while(tmp != NULL){
            nodes_found += 1;
            checksum += tmp->songID;
            tmp = tmp->next;
        }
    }
    if(bst_node_number != (N * N)){
        printf(ANSI_COLOR_RED"total size check failed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET, (N*N), nodes_found);
    }else{
        printf(ANSI_COLOR_GREEN"total size check passed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET, (N*N), nodes_found);
    }
    if(bst_checksum != correct_checksum){
        printf(ANSI_COLOR_RED"total keysum check failed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET,correct_checksum,checksum);
    }else{
        printf(ANSI_COLOR_GREEN"total keysum check passed: (expected: %d, found: %d)\n"ANSI_COLOR_RESET,correct_checksum,checksum);
    }
    return NULL;
}

void* third(void* arg){
    int i = 0;
    int id = (int)arg;
    int N = number_of_threads;
    int M = number_of_threads/2;
    int index = 1;
    int select = 0;
    for(i = 0; i < M; i++){
        //insert_list(i);
        select = (id + index)%(N/2);
        if(debug){
            printf("I am thread (%d) and removed: %d.\n", id, dequeue(select));
        }else{
            dequeue(select);
        }
        
        index += 1;
    }
    pthread_barrier_wait(&third_barrier);
    return NULL;
}

void* third_check(void* arg){
    L_NODE* tmp = my_list->head;
    while(tmp != NULL){
        printf("Found (%d) in the list.\n", tmp->songID);
        tmp = tmp->next;
    }
    return NULL;
}
int main(int argc, char *argv[]){
    int i = 0;
    if(argc < 2){
        printf("Usage: a.out <thread count>\n");
        exit(0);
    }
    if(argc == 3){
        debug = atoi(argv[2]);
    }
    init_tree();
    init_list();
    number_of_threads = atoi(argv[1]);
    printf("Number of threads : %d\n",number_of_threads);
    printf("---------------First Phase---------------\n");
    init_queues(number_of_threads/2);
    pthread_t my_threads[number_of_threads];
    /*Init barriers*/
    pthread_barrier_init(&first_barrier, NULL, number_of_threads);
    pthread_barrier_init(&second_barrier, NULL, number_of_threads);
    /*Start thread for tree insertion*/
    for(i = 0; i < number_of_threads; i++){
        pthread_create(&my_threads[i], NULL,first,(void*)i);
    }
    for(i = 0; i < number_of_threads; i++){
        pthread_join(my_threads[i],NULL);
    }
    pthread_create(&my_threads[0], NULL, first_check, NULL);
    pthread_join(my_threads[0], NULL);
    printf("--------------Second Phase---------------\n");
    for(i = 0; i < number_of_threads; i++){
        pthread_create(&my_threads[i], NULL, second,(void*)i);
    }
    for(i = 0; i < number_of_threads; i++){
        pthread_join(my_threads[i],NULL);
    }
    pthread_create(&my_threads[0], NULL, second_check, NULL);
    pthread_join(my_threads[0], NULL);
    printf("---------------Third Phase---------------\n");
    for(i = 0; i < number_of_threads; i++){
        pthread_create(&my_threads[i], NULL, third, (void*)i);
    }
    for(i = 0; i < number_of_threads; i++){
        pthread_join(my_threads[i],NULL);
    }
    //third_check(NULL);
    second_check(NULL);

    pthread_barrier_destroy(&first_barrier);
    pthread_barrier_destroy(&second_barrier);
    pthread_barrier_destroy(&third_barrier);
    return 0;
}