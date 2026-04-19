#pragma once

#include <stdbool.h>

typedef struct AVLTree AVLTree;

AVLTree* avlCreate(void);
AVLTree* loadBase(const char* filename);
char* avlSearch(AVLTree* tree, const char* code);
void avlInsert(AVLTree* tree, const char* code, const char* name);
void avlDelete(AVLTree* tree, const char* code);
void avlFree(AVLTree* tree);
void avlSave(AVLTree* tree, const char* filename);
int avlSize(AVLTree* tree);