#include "cities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int testsPassed = 0;
static int testsFailed = 0;

#define ASSERT(cond, msg)              \
    do {                               \
        if (cond) {                    \
            testsPassed++;             \
        } else {                       \
            testsFailed++;             \
            printf("FAIL: %s\n", msg); \
        }                              \
    } while (0)

static void runAlgorithmAndCapture(int n, int m, int* edges, int k, int* capitals, char* output, size_t outSize)
{
    Graph graph = createGraph(n);
    for (int i = 0; i < m; i++) {
        int from = edges[i * 3] - 1;
        int to = edges[i * 3 + 1] - 1;
        int len = edges[i * 3 + 2];
        addEdge(&graph, from, to, len);
        addEdge(&graph, to, from, len);
    }

    CityList* states = (CityList*)malloc((size_t)k * sizeof(CityList));
    for (int i = 0; i < k; i++) {
        states[i].cities = NULL;
        states[i].count = 0;
        states[i].capacity = 0;
        addCityToList(&states[i], capitals[i] - 1);
    }

    StatePQ* statePQs = (StatePQ*)malloc((size_t)k * sizeof(StatePQ));
    for (int i = 0; i < k; i++) {
        statePQs[i] = createStatePQ(n, states[i].cities, (int)states[i].count, &graph);
    }

    int* visited = (int*)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    for (int i = 0; i < k; i++) {
        visited[capitals[i] - 1] = 1;
    }

    while (anyUnassigned(n, visited)) {
        for (int s = 0; s < k; s++) {
            int nearest = findNearestCity(&statePQs[s], n, visited);
            if (nearest >= 0) {
                addCityToList(&states[s], nearest);
                visited[nearest] = 1;
                updateStatePQ(&statePQs[s], nearest, &graph);
            }
        }
    }

    char buffer[4096];
    buffer[0] = '\0';
    for (int i = 0; i < k; i++) {
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "%d:", i + 1);
        strcat(buffer, tmp);
        for (size_t j = 0; j < states[i].count; j++) {
            snprintf(tmp, sizeof(tmp), " %d", states[i].cities[j] + 1);
            strcat(buffer, tmp);
        }
        strcat(buffer, "\n");
    }

    strncpy(output, buffer, outSize - 1);
    output[outSize - 1] = '\0';

    for (int i = 0; i < k; i++) {
        freeCityList(&states[i]);
        freeStatePQ(&statePQs[i]);
    }
    free(states);
    free(statePQs);
    free(visited);
    freeGraph(&graph);
}

static void testSingleState()
{
    printf("Test: single state (k=1)\n");
    int n = 4;
    int m = 3;
    int edges[] = { 1, 2, 1, 2, 3, 2, 3, 4, 3 };
    int k = 1;
    int capitals[] = { 1 };

    char output[4096];
    runAlgorithmAndCapture(n, m, edges, k, capitals, output, sizeof(output));

    ASSERT(strstr(output, "1: 1 2 3 4") != NULL, "Single state should contain all cities");
}

static void testEveryCityCapital()
{
    printf("Test: every city is a capital (k=n)\n");
    int n = 3;
    int m = 2;
    int edges[] = { 1, 2, 1, 2, 3, 2 };
    int k = 3;
    int capitals[] = { 1, 2, 3 };

    char output[4096];
    runAlgorithmAndCapture(n, m, edges, k, capitals, output, sizeof(output));

    ASSERT(strstr(output, "1: 1") != NULL, "State 1 should have city 1");
    ASSERT(strstr(output, "2: 2") != NULL, "State 2 should have city 2");
    ASSERT(strstr(output, "3: 3") != NULL, "State 3 should have city 3");
}

static void testTwoStatesBasic()
{
    printf("Test: two states basic\n");
    int n = 4;
    int m = 4;
    int edges[] = { 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 1, 4 };
    int k = 2;
    int capitals[] = { 1, 3 };

    char output[4096];
    runAlgorithmAndCapture(n, m, edges, k, capitals, output, sizeof(output));

    ASSERT(strstr(output, "1: 1 2") != NULL, "State 1 should have cities 1, 2");
    ASSERT(strstr(output, "2: 3 4") != NULL, "State 2 should have cities 3, 4");
}

static void testClosestCityByDistance()
{
    printf("Test: closest city by distance\n");
    int n = 4;
    int m = 4;
    int edges[] = { 1, 2, 5, 1, 3, 1, 2, 4, 2, 3, 4, 3 };
    int k = 2;
    int capitals[] = { 1, 4 };

    char output[4096];
    runAlgorithmAndCapture(n, m, edges, k, capitals, output, sizeof(output));

    ASSERT(strstr(output, "1: 1 3") != NULL, "State 1 should take city 3 (distance 1) before city 2 (distance 5)");
    ASSERT(strstr(output, "2: 4 2") != NULL, "State 2 should take city 2 (distance 2) before nothing else");
}

static void testLinearChain()
{
    printf("Test: linear chain graph\n");
    int n = 5;
    int m = 4;
    int edges[] = { 1, 2, 1, 2, 3, 1, 3, 4, 1, 4, 5, 1 };
    int k = 2;
    int capitals[] = { 1, 5 };

    char output[4096];
    runAlgorithmAndCapture(n, m, edges, k, capitals, output, sizeof(output));

    ASSERT(strstr(output, "1: 1 2 3") != NULL, "State 1 should take 1,2,3");
    ASSERT(strstr(output, "2: 5 4") != NULL, "State 2 should take 5,4");
}

int main()
{
    printf("Running tests\n");

    testSingleState();
    testEveryCityCapital();
    testTwoStatesBasic();
    testClosestCityByDistance();
    testLinearChain();

    printf("\n Tests passed: %d\n", testsPassed);
    printf("Tests failed: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
} 