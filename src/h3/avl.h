#pragma once

#include <stdbool.h>

typedef struct AVLTree AVLTree;

// creates a new tree
AVLTree* avlCreate(void);

// searches for a node with the given key
char* avlSearch(AVLTree* tree, const char* code);

// inserts a new key into the AVL tree
void avlInsert(AVLTree* tree, const char* code, const char* name);

// deletes a node with the given key
void avlDelete(AVLTree* tree, const char* code);

// frees all memory allocated for the AVL tree
void avlFree(AVLTree* tree);

// function for ger size of the avl
int avlSize(AVLTree* tree);

typedef struct AVLIterator AVLIterator;

AVLIterator* avlIteratorCreate(AVLTree* tree);
bool avlIteratorNext(AVLIterator* iter, const char** code, const char** name);
void avlIteratorFree(AVLIterator* iter);