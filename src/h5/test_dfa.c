#include "dfa.h"
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

static int testInvalid()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "823.16.10", &result);
    return result == false ? 0 : 1;
}

static int testInvalidText()
{
    DFA dfa;
    createNumberDFA(&dfa);

    bool result;
    dfaCheckString(&dfa, "abc", &result);
    return result == false ? 0 : 1;
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

int main()
{
    int failures = 0;

    failures += testInteger();
    failures += testFloat();
    failures += testFloatNoIntPart();
    failures += testScientific();
    failures += testNegative();
    failures += testScientificWithSign();
    failures += testInvalid();
    failures += testInvalidText();
    failures += testNullPointer();
    failures += testInvalidSymbol();

    return failures;
}
