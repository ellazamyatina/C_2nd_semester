#include "avl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void saveToFile(AVLTree* tree, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
        return;

    AVLIterator* iter = avlIteratorCreate(tree);
    if (!iter) {
        fclose(file);
        return;
    }

    const char* code;
    const char* name;
    while (avlIteratorNext(iter, &code, &name)) {
        fprintf(file, "%s:%s\n", code, name);
    }
    avlIteratorFree(iter);
    fclose(file);
}

static AVLTree* loadFromFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return NULL;

    AVLTree* tree = avlCreate();
    if (!tree) {
        fclose(file);
        return NULL;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char* colon = strchr(line, ':');
        if (!colon)
            continue;

        *colon = '\0';
        char* code = line;
        char* name = colon + 1;

        if (strlen(code) != 3)
            continue;

        avlInsert(tree, code, name);
    }

    fclose(file);
    return tree;
}

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
    saveToFile(tree, "test.txt");
    avlFree(tree);

    tree = loadFromFile("test.txt");
    assert(tree != NULL);
    assert(avlSearch(tree, "LED") != NULL);
    printf("Save and load work\n");

    avlFree(tree);
    remove("test.txt");

    printf("\nAll tests passed! \n");
    return 0;
}