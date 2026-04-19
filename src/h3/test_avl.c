#include "avl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    printf("Testing AVL AIRPORT Tree...\n");

    // test 1
    AVLTree* tree = avlCreate();
    assert(tree != NULL);

    avlInsert(tree, "SVO", "Sheremetyevo");
    avlInsert(tree, "LED", "Pulkovo");

    char* name = avlSearch(tree, "SVO");
    assert(name != NULL);
    assert(strcmp(name, "Sheremetyevo") == 0);
    printf("Insert and find work\n");

    // test 2
    assert(avlSize(tree) == 2);
    printf("Size is correct\n");

    // test 3
    avlDelete(tree, "SVO");
    assert(avlSearch(tree, "SVO") == NULL);
    assert(avlSize(tree) == 1);
    printf("Delete works\n");

    // test 4
    avlSave(tree, "test.txt");
    avlFree(tree);

    tree = loadBase("test.txt");
    assert(tree != NULL);
    assert(avlSearch(tree, "LED") != NULL);
    printf("Save and load work\n");

    avlFree(tree);
    remove("test.txt");

    printf("\nAll tests passed! \n");
    return 0;
}