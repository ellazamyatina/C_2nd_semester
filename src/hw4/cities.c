#include "cities.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Graph createGraph(int n)
{
    return dijkstraGraphCreate(n);
}

void addEdge(Graph* graph, int from, int to, int len)
{
    dijkstraGraphAddUndirectedEdge(graph, from, to, len);
}

void freeGraph(Graph* graph)
{
    dijkstraGraphFree(graph);
}

typedef struct {
    int city;
    int dist;
} PQItem;

typedef struct {
    PQItem* items;
    int size;
    int capacity;
} PriorityQueue;

static void pqInit(PriorityQueue* pq)
{
    pq->capacity = 256;
    pq->items = (PQItem*)malloc((size_t)pq->capacity * sizeof(PQItem));
    pq->size = 0;
}

static void pqPush(PriorityQueue* pq, int city, int dist)
{
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->items = (PQItem*)realloc(pq->items, (size_t)pq->capacity * sizeof(PQItem));
    }
    int i = pq->size++;
    pq->items[i].city = city;
    pq->items[i].dist = dist;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->items[parent].dist <= pq->items[i].dist) {
            break;
        }
        PQItem temp = pq->items[parent];
        pq->items[parent] = pq->items[i];
        pq->items[i] = temp;
        i = parent;
    }
}

static int pqPop(PriorityQueue* pq, int* city, int* dist)
{
    if (pq->size == 0) {
        return 0;
    }
    *city = pq->items[0].city;
    *dist = pq->items[0].dist;
    pq->size--;
    if (pq->size > 0) {
        pq->items[0] = pq->items[pq->size];
        int i = 0;
        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < pq->size && pq->items[left].dist < pq->items[smallest].dist) {
                smallest = left;
            }
            if (right < pq->size && pq->items[right].dist < pq->items[smallest].dist) {
                smallest = right;
            }
            if (smallest == i) {
                break;
            }
            PQItem temp = pq->items[i];
            pq->items[i] = pq->items[smallest];
            pq->items[smallest] = temp;
            i = smallest;
        }
    }
    return 1;
}

static void pqFree(PriorityQueue* pq)
{
    free(pq->items);
}

StatePQ createStatePQ(int n, int* stateCities, int cityCount, Graph* graph)
{
    StatePQ spq;
    spq.itemsDist = (int*)malloc((size_t)n * sizeof(int));
    spq.pq = malloc(sizeof(PriorityQueue));
    pqInit((PriorityQueue*)spq.pq);

    DIJKResult result = dijkstraComputeMultiSource(graph, stateCities, cityCount);
    for (int i = 0; i < n; i++) {
        spq.itemsDist[i] = result.distances[i];
    }
    dijkstraResultFree(&result);

    for (int i = 0; i < n; i++) {
        if (spq.itemsDist[i] != INT_MAX && spq.itemsDist[i] != 0) {
            pqPush((PriorityQueue*)spq.pq, i, spq.itemsDist[i]);
        }
    }

    return spq;
}

void freeStatePQ(StatePQ* spq)
{
    free(spq->itemsDist);
    pqFree((PriorityQueue*)spq->pq);
    free(spq->pq);
}

int findNearestCity(StatePQ* spq, int n, int* visited)
{
    PriorityQueue* pq = (PriorityQueue*)spq->pq;
    while (pq->size > 0) {
        int city, dist;
        pqPop(pq, &city, &dist);
        if (!visited[city] && dist == spq->itemsDist[city]) {
            return city;
        }
    }
    return -1;
}

void updateStatePQ(StatePQ* spq, int newCity, Graph* graph)
{
    spq->itemsDist[newCity] = 0;
    for (int e = graph->head[newCity]; e != -1; e = graph->edges[e].next) {
        int neighbor = graph->edges[e].to;
        int newDist = spq->itemsDist[newCity] + graph->edges[e].len;
        if (newDist < spq->itemsDist[neighbor]) {
            spq->itemsDist[neighbor] = newDist;
            pqPush((PriorityQueue*)spq->pq, neighbor, newDist);
        }
    }
}

void freeCityList(CityList* list)
{
    if (list) {
        free(list->cities);
        list->cities = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

int addCityToList(CityList* list, int city)
{
    if (list->count >= list->capacity) {
        size_t newCap = list->capacity == 0 ? 4 : list->capacity * 2;
        int* newCities = (int*)realloc(list->cities, newCap * sizeof(int));
        if (!newCities) {
            return -1;
        }
        list->cities = newCities;
        list->capacity = newCap;
    }
    list->cities[list->count++] = city;
    return 0;
}

int anyUnassigned(int n, int* visited)
{
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            return 1;
        }
    }
    return 0;
}

void printStateAllocation(int numStates, int k, CityList* states)
{
    for (int i = 0; i < k; i++) {
        printf("%d:", i + 1);
        if (states[i].count == 0) {
            printf("\n");
            continue;
        }
        for (size_t j = 0; j < states[i].count; j++) {
            printf(" %d", states[i].cities[j] + 1);
        }
        printf("\n");
    }
}