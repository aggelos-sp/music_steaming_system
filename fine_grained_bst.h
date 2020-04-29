#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct bst_node {
  int songID; 
  struct bst_node *lc;
  struct bst_node *rc;
  struct bst_node *p;
  pthread_mutex_t lock;
}T_NODE;

T_NODE* global_root;
pthread_mutex_t tree_lock;

void init_tree();
T_NODE* create_tree_node(int songID, T_NODE* parent);
T_NODE* delete_util(int songID, T_NODE* root);
void insert(int songID, T_NODE* root, T_NODE* parent);
int search(int songID, T_NODE* root, T_NODE* parent);