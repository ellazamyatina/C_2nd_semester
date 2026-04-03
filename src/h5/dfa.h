#pragma once
#include <stdbool.h>
#include <stdint.h>

// error codes returned by dfa
typedef enum {
    DFA_SUCCESS = 0,
    DFA_INVALID_SYMBOL,
    DFA_NULL_POINTER,
    DFA_INVALID_STATE,
    DFA_INVALID_TRANSITION
} DfaErrorCode;

// do a single transition in the dfa
typedef struct
{
    int fromState;
    char symbol;
    int toState;
} Transition;

// represents a dfa
typedef struct
{
    Transition* transitions;
    int transitionCount;

    int* acceptStates;
    int acceptStateCount;

    int startState;
} DFA;

// checks if a given string is accepted by the dfa
DfaErrorCode dfaCheckString(DFA* dfa, const char* input, bool* result);
