#include "dfa.h"
#include <stdio.h>
#include <string.h>

static Transition transitions[] = {
    { 0, '+', 7 }, { 0, '-', 7 },
    { 0, '0', 1 }, { 0, '1', 1 }, { 0, '2', 1 }, { 0, '3', 1 }, { 0, '4', 1 }, { 0, '5', 1 },
    { 0, '6', 1 }, { 0, '7', 1 }, { 0, '8', 1 }, { 0, '9', 1 },
    { 0, '.', 2 },

    { 7, '0', 1 }, { 7, '1', 1 }, { 7, '2', 1 }, { 7, '3', 1 }, { 7, '4', 1 }, { 7, '5', 1 },
    { 7, '6', 1 }, { 7, '7', 1 }, { 7, '8', 1 }, { 7, '9', 1 },

    { 1, '0', 1 }, { 1, '1', 1 }, { 1, '2', 1 }, { 1, '3', 1 }, { 1, '4', 1 }, { 1, '5', 1 },
    { 1, '6', 1 }, { 1, '7', 1 }, { 1, '8', 1 }, { 1, '9', 1 },
    { 1, '.', 2 },
    { 1, 'E', 4 }, { 1, 'e', 4 },

    { 2, '0', 3 }, { 2, '1', 3 }, { 2, '2', 3 }, { 2, '3', 3 }, { 2, '4', 3 }, { 2, '5', 3 },
    { 2, '6', 3 }, { 2, '7', 3 }, { 2, '8', 3 }, { 2, '9', 3 },

    { 3, '0', 3 }, { 3, '1', 3 }, { 3, '2', 3 }, { 3, '3', 3 }, { 3, '4', 3 }, { 3, '5', 3 },
    { 3, '6', 3 }, { 3, '7', 3 }, { 3, '8', 3 }, { 3, '9', 3 },
    { 3, 'E', 4 }, { 3, 'e', 4 },

    { 4, '0', 6 }, { 4, '1', 6 }, { 4, '2', 6 }, { 4, '3', 6 }, { 4, '4', 6 }, { 4, '5', 6 },
    { 4, '6', 6 }, { 4, '7', 6 }, { 4, '8', 6 }, { 4, '9', 6 },
    { 4, '+', 5 }, { 4, '-', 5 },

    { 5, '0', 6 }, { 5, '1', 6 }, { 5, '2', 6 }, { 5, '3', 6 }, { 5, '4', 6 }, { 5, '5', 6 },
    { 5, '6', 6 }, { 5, '7', 6 }, { 5, '8', 6 }, { 5, '9', 6 },

    { 6, '0', 6 }, { 6, '1', 6 }, { 6, '2', 6 }, { 6, '3', 6 }, { 6, '4', 6 }, { 6, '5', 6 },
    { 6, '6', 6 }, { 6, '7', 6 }, { 6, '8', 6 }, { 6, '9', 6 }
};

static int acceptStates[] = { 1, 3, 6 };

static void createNumberDFA(DFA* dfa)
{
    dfa->transitions = transitions;
    dfa->transitionCount = (int)(sizeof(transitions) / sizeof(transitions[0]));
    dfa->acceptStates = acceptStates;
    dfa->acceptStateCount = (int)(sizeof(acceptStates) / sizeof(acceptStates[0]));
    dfa->startState = 0;
}

static int testInteger()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "123", &result);
    return result == true ? 0 : 1;
}

static int testFloat()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "3.14", &result);
    return result == true ? 0 : 1;
}

static int testFloatNoIntPart()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, ".591", &result);
    return result == true ? 0 : 1;
}

static int testScientific()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "38.871E5", &result);
    return result == true ? 0 : 1;
}

static int testNegative()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "-3.14", &result);
    return result == true ? 0 : 1;
}

static int testScientificWithSign()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "1.5e-10", &result);
    return result == true ? 0 : 1;
}

static int testInvalidDoubleE()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "123ee45", &result);
    return error == DFA_INVALID_TRANSITION ? 0 : 1;
}

static int testInvalidPlusAlone()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "+", &result);
    return error == DFA_SUCCESS && result == false ? 0 : 1;
}

static int testInvalidMinusAlone()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "-", &result);
    return error == DFA_SUCCESS && result == false ? 0 : 1;
}

static int testInvalidDoubleDot()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "823.16.10", &result);
    return error == DFA_INVALID_TRANSITION ? 0 : 1;
}

static int testInvalidText()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "abc", &result);
    return (error == DFA_INVALID_SYMBOL || (error == DFA_SUCCESS && result == false)) ? 0 : 1;
}

static int testNullPointer()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "123", NULL);
    return error == DFA_NULL_POINTER ? 0 : 1;
}

static int testInvalidSymbol()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "12a", &result);
    return error == DFA_INVALID_SYMBOL ? 0 : 1;
}

static int testNoTransition()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    DfaErrorCode error = dfaCheckString(&dfa, "123.", &result);
    return error == DFA_SUCCESS && result == false ? 0 : 1;
}

int main()
{
    int failures = 0;

    failures += testInteger();
    failures += testFloat();
    failures += testFloatNoIntPart();
    failures += testScientific();
    failures += testNegative();
    failures += testScientificWithSign();
    failures += testInvalidDoubleE();
    failures += testInvalidPlusAlone();
    failures += testInvalidMinusAlone();
    failures += testInvalidDoubleDot();
    failures += testInvalidText();
    failures += testNullPointer();
    failures += testInvalidSymbol();
    failures += testNoTransition();

    return failures;
}