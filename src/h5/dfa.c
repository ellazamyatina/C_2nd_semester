#include "dfa.h"
#include <stdbool.h>
#include <stddef.h>

static int findTransition(DFA* dfa, int currentState, char symbol)
{
    for (int i = 0; i < dfa->transitionCount; i++) {
        if (dfa->transitions[i].fromState == currentState && dfa->transitions[i].symbol == symbol) {
            return dfa->transitions[i].toState;
        }
    }
    return -1;
}

static bool isAcceptState(DFA* dfa, int state)
{
    for (int i = 0; i < dfa->acceptStateCount; i++) {
        if (dfa->acceptStates[i] == state) {
            return true;
        }
    }
    return false;
}

DfaErrorCode dfaCheckString(DFA* dfa, const char* input, bool* result)
{
    if (dfa == NULL || result == NULL) {
        return DFA_NULL_POINTER;
    }

    if (input == NULL) {
        *result = false;
        return DFA_SUCCESS;
    }

    int currentState = dfa->startState;

    for (int i = 0; input[i] != '\0'; i++) {
        char symbol = input[i];
        int nextState = findTransition(dfa, currentState, symbol);

        if (nextState == -1) {
            *result = false;
            return DFA_INVALID_SYMBOL;
        }

        currentState = nextState;
    }

    *result = isAcceptState(dfa, currentState);
    return DFA_SUCCESS;
}
