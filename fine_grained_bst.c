#include "fine_grained_bst.h"

T_NODE* create_node(int songID){
    T_NODE* node = malloc(sizeof(T_NODE));

    if(node == NULL){
        printf("Error while creating node!!\n");
        return NULL;
    }

    node->songID = songID;
    node->rc = NULL;
    node->lc = NULL;
    pthread_mutex_init(&node->lock, NULL);
    
    return node;
}

void instert(int songID, T_NODE* root, T_NODE* parent){
    if(parent == NULL){
        pthread_mutex_lock(&tree_lock);
        if(global_root == NULL){
            global_root = create_node(songID);
            pthread_mutex_unlock(&tree_lock);
            return;
        }
        pthread_mutex_lock(&global_root->lock);
        root = global_root;
        pthread_mutex_unlock(&tree_lock);
    }
    //id < current id so go left
    if(songID < root->songID){
        if(root->lc == NULL){
            root->lc = create_node(songID);
            pthread_mutex_unlock(&root->lock);
        }else{
            pthread_mutex_lock(&root->lc->lock);
            pthread_mutex_unlock(&root->lock);
            insert(songID,root->lc, root);
        }
    //id > current id so go right
    }else {
        if(root->rc == NULL){
            root->rc = create_node(songID);
            pthread_mutex_unlock(&root->lock);
        }else{
            pthread_mutex_lock(&root->rc->lock);
            pthread_mutex_unlock(&root->lock);
            instert(songID, root->rc, root);
        }
    }
}