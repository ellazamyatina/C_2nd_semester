#include "airport.h"
#include "avl.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static Node* root = NULL;

static void printPrompt(void)
{
    printf("> ");
    fflush(stdout);
}

static void handleFind(const char* code)
{
    if (strlen(code) != 3) {
        printf("Erroк! IATA code must be 3 letters!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!isalpha(code[i])) {
            printf("Error! IATA code must contain only letters!\n");
            return;
        }
    }

    int intCode = IATAToInt(code);
    if (!findAndPrintAirport(root, intCode)) {
        printf("Airport with code '%s' not found in database.\n", code);
    }
}

static void handleAdd(const char* arg)
{
    char* colon = strchr(arg, ':');
    if (!colon) {
        printf("Error! Invalid format!\n");
        return;
    }

    char code[4];
    strncpy(code, arg, colon - arg);
    code[colon - arg] = '\0';
    const char* name = colon + 1;

    if (strlen(code) != 3) {
        printf("Error! IATA code must be 3 letters!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!isalpha(code[i])) {
            printf("Error! IATA code must contain only letters!\n");
            return;
        }
    }

    int intCode = IATAToInt(code);
    root = insertAirport(root, intCode, name);
    printf("Airport '%s' added to database.\n", code);
}

static void handleDelete(const char* code)
{
    if (strlen(code) != 3) {
        printf("Error! IATA code must be 3 letters!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!isalpha(code[i])) {
            printf("Error! IATA code must contain only letters!\n");
            return;
        }
    }

    int intCode = IATAToInt(code);

    if (!searchNode(root, intCode)) {
        printf("Airport with code '%s' not found in database.\n", code);
        return;
    }

    root = deleteNode(root, intCode);
    printf("Airport '%s' deleted from database.\n", code);
}

static void handleSave(const char* filename)
{
    int count = getAirportCount(root);
    saveAirports(root, filename);
    printf("Database saved: %d airports.\n", count);
}

static char* trim(char* str)
{
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';
    return str;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <airports_file>!\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    root = loadAirports(filename);
    if (!root) {
        printf("Failed to load database! Program terminated!\n");
        return 1;
    }

    char line[1024];
    while (true) {
        printPrompt();

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\r\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        char* command = line;
        char* arg = strchr(line, ' ');

        if (arg) {
            *arg = '\0';
            arg++;
            arg = trim(arg);
        }

        command = trim(command);

        if (strcmp(command, "find") == 0) {
            if (!arg) {
                printf("Error! find command requires IATA code!\n");
                continue;
            }
            handleFind(arg);
        } else if (strcmp(command, "add") == 0) {
            if (!arg) {
                printf("Error! add command requires <code>:<name>!\n");
                continue;
            }
            handleAdd(arg);
        } else if (strcmp(command, "delete") == 0) {
            if (!arg) {
                printf("Error! delete command requires IATA code!\n");
                continue;
            }
            handleDelete(arg);
        } else if (strcmp(command, "save") == 0) {
            handleSave(filename);
        } else if (strcmp(command, "quit") == 0) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Unknown command! Available: find, add, delete, save, quit\n");
        }

        printf("\n");
    }

    freeAirports(root);

    return 0;
}
