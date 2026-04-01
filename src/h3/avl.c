#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

Node* createNewNode(int value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = value;
    node->balance = 0;
    node->left = node->right = NULL;
    return node;
};

Node* rotateLeft(Node* a)
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

Node* rotateRight(Node* a)
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

Node* bigRotateLeft(Node* node)
{
    node->right = rotateRight(node->right);
    Node* newRoot = rotateLeft(node);
    newRoot->balance = 0;
    return newRoot;
}

Node* bigRotateRight(Node* node)
{
    node->left = rotateLeft(node->left);
    Node* newRoot = rotateRight(node);
    newRoot->balance = 0;
    return newRoot;
}

Node* balance(Node* node)
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

Node* searchNode(Node* root, int value)
{
    if ((root == NULL || root->key == value)) {
        return root;
    }
    if (root->key < value) {
        return searchNode(root->right, value);
    }
    return searchNode(root->left, value);
}

Node* insertNode(Node* node, int value)
{
    if (node == NULL) {
        return createNewNode(value);
    }
    if (value < node->key) {
        node->left = insertNode(node->left, value);
        node->balance += -1; // insert in the left tree
    } else if (value > node->key) {
        node->right = insertNode(node->right, value);
        node->balance += 1; // insert in the right tree
    } else {
        return node;
    } // dublicates
    return balance(node);
}

void postOrder(Node* root)
{ //
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d(balance:%d)\r\n", root->key, root->balance);
    }
}

void inOrder(Node* root)
{
    if (root != NULL) {
        inOrder(root->left);
        printf("%d(balance:%d)\r\n", root->key, root->balance);
        inOrder(root->right);
    }
}

void preOrder(Node* root)
{
    if (root != NULL) {
        printf("%d(balance:%d)\r\n", root->key, root->balance);
        preOrder(root->left);
        preOrder(root->right);
    }
}

Node* findMin(Node* root)
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node* delete (Node* root, int key)
{
    if (root == NULL) {
        return NULL;
    }
    if (key < root->key) {
        root->left = delete (root->left, key);
    } else if (key > root->key) {
        root->right = delete (root->right, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL || root->right == NULL) {
            Node* ptr;
            if (root->left == NULL) {
                ptr = root->right;
            } else {
                ptr = root->left;
            }
            free(root);
            return ptr;
        } else {
            Node* ptr = findMin(root->right);
            root->key = ptr->key;
            root->right = delete (root->right, ptr->key);
        }
    }
    int leftHeight = 0;
    int rightHeight = 0;
    if (root->left != NULL) {
        if (root->left->balance > 0)
            leftHeight = 2;
        else if (root->left->balance < 0)
            leftHeight = 0;
        else
            leftHeight = 1;
    }

    if (root->right != NULL) {
        if (root->right->balance > 0)
            rightHeight = 2;
        else if (root->right->balance < 0)
            rightHeight = 0;
        else
            rightHeight = 1;
    }

    root->balance = rightHeight - leftHeight;

    return balance(root);
}

void freeTree(Node* root)
{
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}