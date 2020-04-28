#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node {
  int songID; 
  struct bst_node *lc;
  struct bst_node *rc;
  pthread_mutex_t lock;
}T_NODE;

T_NODE* global_root;
pthread_mutex_t tree_lock;

void init_tree();
T_NODE* create_tree_node(int songID);
T_NODE* search(int songID, T_NODE* root);
void insert(int songID, T_NODE* root, T_NODE* parent);
