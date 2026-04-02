#include "cities.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        fprintf(stderr, "Error: invalid input format for n and m\n");
        return 1;
    }

    if (n <= 0 || m < 0) {
        fprintf(stderr, "Error: n must be positive and m must be non-negative\n");
        return 1;
    }

    Graph graph = createGraph(n);

    for (int i = 0; i < m; i++) {
        int a, b, len;
        if (scanf("%d %d %d", &a, &b, &len) != 3) {
            fprintf(stderr, "Error: invalid edge format at line %d\n", i + 1);
            freeGraph(&graph);
            return 1;
        }
        if (a < 1 || a > n || b < 1 || b > n || len < 0) {
            fprintf(stderr, "Error: invalid edge values (vertices must be 1..%d, length must be non-negative)\n", n);
            freeGraph(&graph);
            return 1;
        }
        a--;
        b--;
        addEdge(&graph, a, b, len);
        addEdge(&graph, b, a, len);
    }

    int k;
    if (scanf("%d", &k) != 1) {
        fprintf(stderr, "Error: invalid input for k\n");
        freeGraph(&graph);
        return 1;
    }

    if (k <= 0 || k > n) {
        fprintf(stderr, "Error: k must be between 1 and n\n");
        freeGraph(&graph);
        return 1;
    }

    int* capitals = (int*)malloc((size_t)k * sizeof(int));
    for (int i = 0; i < k; i++) {
        if (scanf("%d", &capitals[i]) != 1) {
            fprintf(stderr, "Error: invalid capital number at position %d\n", i + 1);
            free(capitals);
            freeGraph(&graph);
            return 1;
        }
        if (capitals[i] < 1 || capitals[i] > n) {
            fprintf(stderr, "Error: capital number must be between 1 and %d\n", n);
            free(capitals);
            freeGraph(&graph);
            return 1;
        }
        capitals[i]--;
    }

    CityList* states = (CityList*)malloc((size_t)k * sizeof(CityList));
    for (int i = 0; i < k; i++) {
        states[i].cities = NULL;
        states[i].count = 0;
        states[i].capacity = 0;
        addCityToList(&states[i], capitals[i]);
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
        visited[capitals[i]] = 1;
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

    printStateAllocation(n, k, states);

    free(capitals);
    for (int i = 0; i < k; i++) {
        freeCityList(&states[i]);
        freeStatePQ(&statePQs[i]);
    }
    free(states);
    free(statePQs);
    free(visited);
    freeGraph(&graph);

    return 0;
} 