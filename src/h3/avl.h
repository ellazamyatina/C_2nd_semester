#pragma once

#include <stdbool.h>

typedef struct AVLTree AVLTree;

// creates a new tree 
AVLTree* avl_create(void);

// searches for a node with the given key
void* avl_search(AVLTree* tree, int value);

// inserts a new key into the AVL tree
void avl_insert(AVLTree* tree, int value, void* data);

// deletes a node with the given key
void avl_delete(AVLTree* tree, int key);

// frees all memory allocated for the AVL tree
void avl_free(AVLTree* tree);