#pragma once

#include <stdbool.h>

typedef struct Node {
    int key;
    int balance;
    struct Node* left;
    struct Node* right;
    void* data;
} Node;

// creates a new node with the given key value
struct Node* createNewNode(int value);

// performs a left rotation on the given node
struct Node* rotateLeft(Node* a);

// performs a right rotation on the given node
struct Node* rotateRight(Node* a);

// performs a big left rotation
struct Node* bigRotateLeft(Node* node);

// performs a big right rotation
struct Node* bigRotateRight(Node* node);

// balances the AVL tree starting from the given node
struct Node* balance(Node* node);

// searches for a node with the given key
struct Node* searchNode(Node* root, int value);

// inserts a new key into the AVL tree
struct Node* insertNode(Node* node, int value);

// performs post-order traversal of the tree
void postOrder(Node* root);

// performs in-order traversal of the tree
void inOrder(Node* root);

// performs pre-order traversal of the tree
void preOrder(Node* root);

// finds the node with the minimum key in the tree
struct Node* findMin(Node* root);

// deletes a node with the given key
struct Node* deleteNode(Node* root, int key);

// frees all memory allocated for the AVL tree
void freeTree(Node* root);