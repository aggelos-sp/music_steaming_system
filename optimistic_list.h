#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define true 1
#define false 0

typedef struct optimistic_list {
    L_NODE* head;
    L_NODE* tail;
}LIST;

typedef struct list_node {
  int songID; 
  struct list_node *next;
  pthread_mutex_t lock;
}L_NODE;

LIST* my_list;
pthread_mutex_t list_lock;

void list_init();
L_NODE* create_list_node(int songID);
int validate(L_NODE* pred, L_NODE* curr);
int insert_list(int songID);

