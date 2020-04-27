#include "fine_grained_bst.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN     "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int number_of_threads = 0;
int bst_node_number = 0;
int bst_checksum = 0;
pthread_barrier_t first_b;

void* generate_songs(void *id){
    printf("I am in\n");
    int i = 0;
    int song_id = 0;
    for(i = 0; i < number_of_threads; i++){
        song_id = ((i * number_of_threads) + (int)id);
        printf("I am thread: %d and going to insert:%d\n",(int)id,song_id);
        insert(song_id,global_root,NULL);
    }
    pthread_barrier_wait(&first_b);
}
void tree_counter(T_NODE* node) 
{ 
    if (node == NULL) 
        return;
    tree_counter(node->lc); 

    bst_node_number++;
    bst_checksum += node->songID; 

    tree_counter(node->rc); 
} 
void first_check(void * arg){
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
    
}
int main(int argc, char *argv[]){
    
    if(argc != 2){
        printf("Usage: a.out <thread count>\n");
        exit(0);
    }
    int i = 0;
    printf("Init tree start\n");
    init_tree();
    printf("Init tree end\n");
    number_of_threads = atoi(argv[1]);
    printf("Number of threads %d\n",number_of_threads);
    pthread_t my_threads[number_of_threads];
    pthread_barrier_init(&first_b, NULL, number_of_threads);
    for(i = 0; i < number_of_threads; i++){
        pthread_create(&my_threads[i], NULL,(void*)generate_songs,(void*)i);
    }
    for(i = 0; i < number_of_threads; i++){
        pthread_join(my_threads[i],NULL);
    }
    first_check(NULL);
    pthread_barrier_destroy(&first_b);
    return 0;
}