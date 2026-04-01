#include "airport.h"
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AirportData {
    char* name;
} AirportData;

int IATAToInt(const char* code)
{
    if (!code || strlen(code) < 3)
        return 0;
    return (code[0] << 16) | (code[1] << 8) | code[2];
}

void intToIata(int code, char* buffer)
{
    if (!buffer)
        return;
    buffer[0] = (code >> 16) & 0xFF;
    buffer[1] = (code >> 8) & 0xFF;
    buffer[2] = code & 0xFF;
    buffer[3] = '\0';
}

static Node* insertAirportInternal(Node* root, int code, const char* name)
{
    Node* existing = searchNode(root, code);
    if (existing && existing->data) {
        AirportData* data = (AirportData*)existing->data;
        free(data->name);
        data->name = strdup(name);
        return root;
    }

    root = insertNode(root, code);
    Node* node = searchNode(root, code);
    if (node) {
        AirportData* data = (AirportData*)malloc(sizeof(AirportData));
        data->name = strdup(name);
        node->data = data;
    }

    return root;
}

Node* insertAirport(Node* root, int code, const char* name)
{
    return insertAirportInternal(root, code, name);
}

int findAndPrintAirport(Node* root, int code)
{
    Node* node = searchNode(root, code);
    if (!node || !node->data) {
        return 0;
    }

    AirportData* data = (AirportData*)node->data;
    char iata[4];
    intToIata(code, iata);
    printf("%s → %s\n", iata, data->name);
    return 1;
}

Node* loadAirports(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error! Cannot open file! %s\n", filename);
        return NULL;
    }

    Node* root = NULL;
    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* colon = strchr(line, ':');
        if (!colon)
            continue;

        *colon = '\0';
        const char* code = line;
        const char* name = colon + 1;

        if (strlen(code) == 0)
            continue;

        int intCode = IATAToInt(code);
        root = insertAirport(root, intCode, name);
        count++;
    }

    fclose(file);
    printf("Loaded %d airports. System ready to work.\n", count);
    return root;
}

static void saveAirportsRecursive(Node* node, FILE* file)
{
    if (!node)
        return;

    saveAirportsRecursive(node->left, file);

    if (node->data) {
        AirportData* data = (AirportData*)node->data;
        char iata[4];
        intToIata(node->key, iata);
        fprintf(file, "%s:%s\n", iata, data->name);
    }

    saveAirportsRecursive(node->right, file);
}

void saveAirports(Node* root, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error! Cannot save file! %s\n", filename);
        return;
    }

    saveAirportsRecursive(root, file);
    fclose(file);
}

static int countAirportsRecursive(Node* node)
{
    if (!node)
        return 0;
    return 1 + countAirportsRecursive(node->left) + countAirportsRecursive(node->right);
}

int getAirportCount(Node* root)
{
    return countAirportsRecursive(root);
}

void freeAirports(Node* root)
{
    if (!root)
        return;

    freeAirports(root->left);
    freeAirports(root->right);

    if (root->data) {
        AirportData* data = (AirportData*)root->data;
        free(data->name);
        free(data);
    }

    free(root);
}