/*
*   Operating System Lab
*       Lab2 (Synchronization)
*       Student id : 
*       Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

pthread_mutex_t mutex;
int node_count = 0;

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
    inorder(tree->root);
    printf("\n node Number : %d",node_count);
    return 0;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree));
    pthread_mutex_init(&tree->mutex, NULL);
    tree->root = NULL;
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
    lab2_node *n = (lab2_node *)malloc(sizeof(lab2_node));
    pthread_mutex_init(&n->mutex, NULL);
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
    lab2_node *p = NULL,
              *t = tree->root,
              *n;
    int key = new_node->key;
    while(t != NULL){
        if(key == t->key) {
            // printf("key == t->key \n");
            node_count++;
            return 0;
        }
        p = t;
        t = key < t->key ? t->left : t->right;
    }
    n = new_node;
    if(p == NULL) {
        tree->root = n;
        // printf("t = new_node; \n");
        node_count++;
        return 0;
    }
    if(key < p->key){
        p->left = n;
    } else {
        p->right = n;
    }
    // printf("insert key : %d\n", key);
    node_count++;
    return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
    lab2_node *p = NULL,
              *t = tree->root,
              *n;
    int key = new_node->key;
    while(t != NULL){
        if(key == t->key) {
            pthread_mutex_lock(&tree->mutex);
            node_count++;
            pthread_mutex_unlock(&tree->mutex);
            return 0;
        }
        p = t;
        pthread_mutex_lock(&p->mutex);
        t = key < t->key ? t->left : t->right;
        pthread_mutex_unlock(&p->mutex);
    }
    n = new_node;
    if(p == NULL) {
        pthread_mutex_lock(&tree->mutex);
        tree->root = n;
        node_count++;
        pthread_mutex_unlock(&tree->mutex);
        return 0;
    }
    pthread_mutex_lock(&tree->mutex);
    if(key < p->key){
        p->left = n;
    } else {
        p->right = n;
    }
    node_count++;
    pthread_mutex_unlock(&tree->mutex);
    return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
    pthread_mutex_lock(&tree->mutex);
    lab2_node *p = NULL,
              *t = tree->root,
              *n;
    int key = new_node->key;
    while(t != NULL){
        if(key == t->key) {
            // printf("key == t->key \n");
            node_count++;
            pthread_mutex_unlock(&tree->mutex);
            return 0;
        }
        p = t;
        t = key < t->key ? t->left : t->right;
    }
    n = new_node;
    if(p == NULL) {
        tree->root = n;
        // printf("t = new_node; \n");
        node_count++;
        pthread_mutex_unlock(&tree->mutex);
        return 0;
    }
    if(key < p->key){
        p->left = n;
    } else {
        p->right = n;
    }
    // printf("insert key : %d\n", key);z
    node_count++;
    pthread_mutex_unlock(&tree->mutex);
    return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
    // c
    // c
    lab2_node *p = NULL, *child, *succ, *succ_p, *t = tree->root;

    if(t == NULL) return 0;
    while(t->key != key){
        //printf("left : %d, right : %d\n", t->left->key, t->right->left->key);
        p = t;
        t = (key < t->key) ? t->left : t->right;
    }
    if( (t->left == NULL) && (t->right == NULL) ){
        if(p != NULL){
            if( p->left == t ){
                p->left = NULL;
            } else{
                p->right = NULL;
            }
        } else{
        
            tree->root = NULL;
        }
    } else if( (t->left == NULL) || (t->right == NULL) ){
        child = (t->left != NULL) ? t->left : t->right;
        if( p != NULL ){
            if( p->left == t ){
            
                p->left = child;
            }
            else{
            
                p->right = child;
            }
        }
        else{ 
        
            tree->root = child;
        }
    } else {
        succ_p = t;
    
        succ = t->right;
        while( succ->left != NULL ){
        
            succ_p = succ;
            succ = succ->left;
        }
        
        if( succ_p->left == succ ){
            succ_p->left = succ->right;
        }
        else{
            succ_p->right = succ->right;
        }
        t->key = succ->key;
        t = succ;
    }
    t=NULL;
    //lab2_node_print_inorder(tree);
    return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
 lab2_node *p = NULL, *child, *succ, *succ_p, *t = tree->root;

    if(t == NULL) return 0;
    while(t->key != key){
        //printf("left : %d, right : %d\n", t->left->key, t->right->left->key);
        p = t;
        t = (key < t->key) ? t->left : t->right;
    }
    if( (t->left == NULL) && (t->right == NULL) ){
        if(p != NULL){
            if( p->left == t ){
                p->left = NULL;
            } else{
                p->right = NULL;
            }
        } else{
        
            tree->root = NULL;
        }
    } else if( (t->left == NULL) || (t->right == NULL) ){
        child = (t->left != NULL) ? t->left : t->right;
        if( p != NULL ){
            if( p->left == t ){
            
                p->left = child;
            }
            else{
            
                p->right = child;
            }
        }
        else{ 
        
            tree->root = child;
        }
    } else {
        succ_p = t;
    
        succ = t->right;
        while( succ->left != NULL ){
        
            succ_p = succ;
            succ = succ->left;
        }
        
        if( succ_p->left == succ ){
            succ_p->left = succ->right;
        }
        else{
            succ_p->right = succ->right;
        }
        t->key = succ->key;
        t = succ;
    }
    t=NULL;
    //lab2_node_print_inorder(tree);
    return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
    pthread_mutex_lock(&tree->mutex);
    lab2_node *p = NULL, *child, *succ, *succ_p, *t = tree->root;
    if(t == NULL){
        pthread_mutex_unlock(&tree->mutex);
        return 0;
    }
    while(t->key != key){
        //printf("left : %d, right : %d\n", t->left->key, t->right->left->key);
        p = t;
        t = (key < t->key) ? t->left : t->right;
    }
    if( (t->left == NULL) && (t->right == NULL) ){
        if(p != NULL){
            if( p->left == t ){
                p->left = NULL;
            } else{
                p->right = NULL;
            }
        } else{ 
            tree->root = NULL;
        }
    } else if( (t->left == NULL) || (t->right == NULL) ){
        child = (t->left != NULL) ? t->left : t->right;
        if( p != NULL ){
            if( p->left == t ){
                p->left = child;
            }
            else{
                p->right = child;
            }
        }
        else{ 
            tree->root = child;
        }
    } else {
        succ_p = t;
        succ = t->right;
        while( succ->left != NULL ){
            succ_p = succ;
            succ = succ->left;
        }
        
        if( succ_p->left == succ ){
            succ_p->left = succ->right;
        }
        else{
            succ_p->right = succ->right;
        }
        
        t->key = succ->key;
        t = succ;
    }
    t=NULL;
    pthread_mutex_unlock(&tree->mutex);
    return 0;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
    int key;
    lab2_node *t = tree->root;
    node_count = 0;
    if(t == NULL) return;
    while(t != NULL){
        key = t->key;
        //printf("key : %d\n",key);
        lab2_node_remove(tree, key);
        t = tree->root;
    }
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
}

 

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void inorder(lab2_node *node){  
    if(node == NULL) return;
    inorder(node->left);
    //printf("%d ", node->key);
    inorder(node->right);
}