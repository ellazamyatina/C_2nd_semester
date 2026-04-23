#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* code;
    char* name;
    int balance;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct AVLTree {
    Node* root;
    int size;
} AVLTree;

static Node* createNewNode(const char* code, const char* name)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;

    node->code = strdup(code);
    node->name = strdup(name);
    node->balance = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void freeNode(Node* node)
{
    if (node) {
        free(node->code);
        free(node->name);
        free(node);
    }
}

static Node* rotateLeft(Node* a)
{
    Node* b = a->right;
    Node* c = b->left;
    b->left = a;
    a->right = c;
    if (c == NULL) {
        a->balance = 0;
        b->balance = 0;
    } else {
        if (c->balance == 0) {
            a->balance = 1;
            b->balance = -1;
        } else if (c->balance > 0) {
            a->balance = 0;
            b->balance = -1;
        } else {
            a->balance = 1;
            b->balance = 0;
        }
    }
    return b;
}

static Node* rotateRight(Node* a)
{
    Node* b = a->left;
    Node* c = b->right;
    b->right = a;
    a->left = c;
    if (c == NULL) {
        a->balance = 0;
        b->balance = 0;
    } else {
        if (c->balance == 0) {
            a->balance = -1;
            b->balance = 1;
        } else if (c->balance > 0) {
            a->balance = 0;
            b->balance = 1;
        } else {
            a->balance = -1;
            b->balance = 0;
        }
    }
    return b;
}

static Node* bigRotateLeft(Node* node)
{
    node->right = rotateRight(node->right);
    Node* newRoot = rotateLeft(node);
    newRoot->balance = 0;
    return newRoot;
}

static Node* bigRotateRight(Node* node)
{
    node->left = rotateLeft(node->left);
    Node* newRoot = rotateRight(node);
    newRoot->balance = 0;
    return newRoot;
}

static Node* balance(Node* node)
{
    if (node->balance == 2) { /// right subtree >
        if (node->right->balance >= 0) {
            return rotateLeft(node);
        }
        return bigRotateLeft(node);
    }
    if (node->balance == -2) {
        if (node->left->balance <= 0) {
            return rotateRight(node);
        }
        return bigRotateRight(node);
    }
    return node;
}

static Node* searchNode(Node* root, const char* code)
{
    while (root != NULL) {
        int cmp = strcmp(code, root->code);
        if (cmp == 0) {
            return root;
        } else if (cmp < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

static Node* insertNode(Node* node, const char* code, const char* name)
{
    if (node == NULL) {
        return createNewNode(code, name);
    }
    int tmp = strcmp(code, node->code);
    if (tmp < 0) {
        node->left = insertNode(node->left, code, name);
        node->balance--;
    } else if (tmp > 0) {
        node->right = insertNode(node->right, code, name);
        node->balance++;
    } else { // dublicate
        free(node->name);
        node->name = strdup(name);
        return node;
    }
    return balance(node);
}

static Node* findMin(Node* node)
{
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

static Node* deleteNode(Node* root, const char* code)
{
    if (root == NULL) {
        return NULL;
    }
    int tmp = strcmp(code, root->code);
    if (tmp < 0) {
        root->left = deleteNode(root->left, code);
    } else if (tmp > 0) {
        root->right = deleteNode(root->right, code);
    } else {
        if (root->left == NULL && root->right == NULL) {
            freeNode(root);
            return NULL;
        } else if (root->left == NULL || root->right == NULL) {
            Node* tmp2 = root->left ? root->left : root->right;
            freeNode(root);
            return tmp2;
        } else {
            Node* min = findMin(root->right);
            free(root->code);
            free(root->name);
            root->code = strdup(min->code);
            root->name = strdup(min->name);
            root->right = deleteNode(root->right, min->code);
        }
    }

    return balance(root);
}

void freeTree(Node* node)
{
    if (node == NULL) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    freeNode(node);
}

AVLTree* avlCreate(void)
{
    AVLTree* tree = malloc(sizeof(AVLTree));
    if (!tree)
        return NULL;

    tree->root = NULL;
    tree->size = 0;
    return tree;
}

char* avlSearch(AVLTree* tree, const char* code)
{
    if (!tree || !code)
        return NULL;

    Node* node = searchNode(tree->root, code);
    return node ? node->name : NULL;
}

void avlInsert(AVLTree* tree, const char* code, const char* name)
{
    if (!tree || !code || !name)
        return;

    if (avlSearch(tree, code) != NULL) {
        return;
    }

    tree->root = insertNode(tree->root, code, name);
    tree->size++;
}

void avlDelete(AVLTree* tree, const char* code)
{
    if (!tree || !code)
        return;

    if (searchNode(tree->root, code) != NULL) {
        tree->root = deleteNode(tree->root, code);
        tree->size--;
    }
}

void avlFree(AVLTree* tree)
{
    if (!tree)
        return;
    freeTree(tree->root);
    free(tree);
}

int avlSize(AVLTree* tree)
{
    return tree ? tree->size : 0;
}

typedef struct AVLIterator {
    Node** stack;
    int top;
    int capacity;
} AVLIterator;

AVLIterator* avlIteratorCreate(AVLTree* tree)
{
    if (!tree || !tree->root)
        return NULL;

    AVLIterator* iter = malloc(sizeof(AVLIterator));
    if (!iter)
        return NULL;

    iter->capacity = 64;
    iter->stack = malloc(sizeof(Node*) * iter->capacity);
    if (!iter->stack) {
        free(iter);
        return NULL;
    }

    iter->top = 0;
    iter->stack[iter->top++] = tree->root;

    return iter;
}

bool avlIteratorNext(AVLIterator* iter, const char** code, const char** name)
{
    if (!iter || iter->top == 0)
        return false;

    Node* node = iter->stack[--iter->top];

    if (code)
        *code = node->code;
    if (name)
        *name = node->name;

    if (node->left) {
        Node* curr = node->left;
        while (curr) {
            iter->stack[iter->top++] = curr;
            curr = curr->right;
        }
    }

    return true;
}

void avlIteratorFree(AVLIterator* iter)
{
    if (!iter)
        return;
    free(iter->stack);
    free(iter);
}