#include "dfa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Transition transitions[] = {
    { 0, '+', 1 }, { 0, '-', 1 },
    { 0, '0', 1 }, { 0, '1', 1 }, { 0, '2', 1 }, { 0, '3', 1 }, { 0, '4', 1 }, { 0, '5', 1 },
    { 0, '6', 1 }, { 0, '7', 1 }, { 0, '8', 1 }, { 0, '9', 1 },
    { 0, '.', 2 },

    { 1, '0', 1 }, { 1, '1', 1 }, { 1, '2', 1 }, { 1, '3', 1 }, { 1, '4', 1 }, { 1, '5', 1 },
    { 1, '6', 1 }, { 1, '7', 1 }, { 1, '8', 1 }, { 1, '9', 1 },
    { 1, '.', 2 },
    { 1, 'E', 3 }, { 1, 'e', 3 },

    { 2, '0', 4 }, { 2, '1', 4 }, { 2, '2', 4 }, { 2, '3', 4 }, { 2, '4', 4 }, { 2, '5', 4 },
    { 2, '6', 4 }, { 2, '7', 4 }, { 2, '8', 4 }, { 2, '9', 4 },

    { 3, '0', 4 }, { 3, '1', 4 }, { 3, '2', 4 }, { 3, '3', 4 }, { 3, '4', 4 }, { 3, '5', 4 },
    { 3, '6', 4 }, { 3, '7', 4 }, { 3, '8', 4 }, { 3, '9', 4 },
    { 3, 'E', 5 }, { 3, 'e', 5 },

    { 4, '0', 4 }, { 4, '1', 4 }, { 4, '2', 4 }, { 4, '3', 4 }, { 4, '4', 4 }, { 4, '5', 4 },
    { 4, '6', 4 }, { 4, '7', 4 }, { 4, '8', 4 }, { 4, '9', 4 },
    { 4, 'E', 5 }, { 4, 'e', 5 },

    { 5, '+', 6 }, { 5, '-', 6 },
    { 5, '0', 6 }, { 5, '1', 6 }, { 5, '2', 6 }, { 5, '3', 6 }, { 5, '4', 6 }, { 5, '5', 6 },
    { 5, '6', 6 }, { 5, '7', 6 }, { 5, '8', 6 }, { 5, '9', 6 },

    { 6, '0', 6 }, { 6, '1', 6 }, { 6, '2', 6 }, { 6, '3', 6 }, { 6, '4', 6 }, { 6, '5', 6 },
    { 6, '6', 6 }, { 6, '7', 6 }, { 6, '8', 6 }, { 6, '9', 6 }
};

static int acceptStates[] = { 1, 4, 6 };

static void createNumberDFA(DFA* dfa)
{
    dfa->transitions = transitions;
    dfa->transitionCount = (int)(sizeof(transitions) / sizeof(transitions[0]));
    dfa->acceptStates = acceptStates;
    dfa->acceptStateCount = (int)(sizeof(acceptStates) / sizeof(acceptStates[0]));
    dfa->startState = 0;
}

int main()
{
    DFA dfa;
    createNumberDFA(&dfa);

    char input[256];
    printf("Введите строку для проверки: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, input, &result);

    if (error == DFA_SUCCESS || error == DFA_INVALID_SYMBOL) {
        printf("%s\n", result ? "Да" : "Нет");
    } else {
        printf("Ошибка: %d\n", error);
    }

    return 0;
}
