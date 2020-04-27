#include "fine_grained_bst.h"

void init_tree(){
    pthread_mutex_init(&tree_lock, NULL);
    global_root = NULL;
}

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

T_NODE* search(int songID, T_NODE* root){
    if(root->songID == songID){
        return root;
    }else if(root->songID < songID){
        if(root->lc == NULL){
            pthread_mutex_unlock(&root->lock);
            return NULL;
        }else{
            pthread_mutex_lock(root->lc->songID);
            if(root->lc->songID == songID){
                return root->lc;
            }
            else{
                pthread_mutex_unlock(&root->lock);
                return search(songID, root->lc);
            }
        }
    }else{
        if(root->rc == NULL){
            pthread_mutex_unlock(&root->lock);
            return NULL;
        }else{
            pthread_mutex_lock(&root->rc->lock);
            if(root->rc->songID == songID){
                return root->rc;
            }else{
                pthread_mutex_unlock(&root->lock);
                return search(songID, root->rc);
            }
        }
    }
    return NULL;
}

int remove(int songID, T_NODE* root){
    
    //pthread_mutex_lock(&tree_lock);
    /**
     * check if the tree is empty
    */
   /*
    if(global_root == NULL){
        pthread_mutex_unlock(&tree_lock);
        return 1;
    }
    */
    /**
     * check if we are lucky and we have to remove the root
     * and is also the only node in the tree
    */
   /*
    if(root->songID == songID && root->lc == NULL && root->rc == NULL){
        free(root);
        global_root = NULL;
        pthread_mutex_unlock(&tree_lock);
        return 0;
    }
    */
    return 0;
}