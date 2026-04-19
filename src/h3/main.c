#include "avl.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void printPrompt(void)
{
    printf("> ");
    fflush(stdout);
}

static bool isValidCode(const char* code)
{
    if (strlen(code) != 3)
        return false;

    for (int i = 0; i < 3; i++) {
        if (!isalpha((unsigned char)code[i])) {
            return false;
        }
    }
    return true;
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
        printf("Usage: %s <airports_file>\n", argv[0]);
        return 1;
    }
    AVLTree* tree = loadBase(argv[1]);
    if (!tree) {
        printf("Failed to load database!\n");
        return 1;
    }

    char line[1024];

    while (1) {
        printPrompt();

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\r\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        // Парсим команду
        char* command = line;
        char* arg = strchr(line, ' ');

        if (arg) {
            *arg = '\0';
            arg++;
            arg = trim(arg);
        }

        command = trim(command);

        if (strcmp(command, "find") == 0) {
            if (!arg || !isValidCode(arg)) {
                printf("Error! find requires valid 3-letter IATA code!\n");
                continue;
            }

            char* name = avlSearch(tree, arg);
            if (name) {
                printf("%s → %s\n", arg, name);
            } else {
                printf("Airport with code '%s' not found in database.\n", arg);
            }

        } else if (strcmp(command, "add") == 0) {
            if (!arg) {
                printf("Error! add requires <code>:<name>!\n");
                continue;
            }

            char* colon = strchr(arg, ':');
            if (!colon) {
                printf("Error! Invalid format. Use <code>:<name>\n");
                continue;
            }

            *colon = '\0';
            char* code = trim(arg);
            char* name = trim(colon + 1);

            if (!isValidCode(code)) {
                printf("Error! IATA code must be 3 letters!\n");
                continue;
            }

            if (strlen(name) == 0) {
                printf("Error! Airport name cannot be empty!\n");
                continue;
            }

            avlInsert(tree, code, name);
            printf("Airport '%s' added to database.\n", code);

        } else if (strcmp(command, "delete") == 0) {
            if (!arg || !isValidCode(arg)) {
                printf("Error! delete requires valid 3-letter IATA code!\n");
                continue;
            }

            if (avlSearch(tree, arg) == NULL) {
                printf("Airport with code '%s' not found in database.\n", arg);
                continue;
            }

            avlDelete(tree, arg);
            printf("Airport '%s' deleted from database.\n", arg);

        } else if (strcmp(command, "save") == 0) {
            avlSave(tree, argv[1]);
            printf("Database saved: %d airports.\n", avlSize(tree));

        } else if (strcmp(command, "quit") == 0) {
            printf("Goodbye!\n");
            break;

        } else {
            printf("Unknown command! Available: find, add, delete, save, quit\n");
        }
    }

    avlFree(tree);
    return 0;
}