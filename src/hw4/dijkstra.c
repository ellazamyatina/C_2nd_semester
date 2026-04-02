#include "dijkstra.h"
#include <limits.h>
#include <stdlib.h>

typedef struct {
    int vertex;
    int dist;
} DIJKPQItem;

typedef struct {
    DIJKPQItem* items;
    int size;
    int capacity;
} DIJKPriorityQueue;

static void dijkstraPQInit(DIJKPriorityQueue* pq)
{
    pq->capacity = 256;
    pq->items = (DIJKPQItem*)malloc((size_t)pq->capacity * sizeof(DIJKPQItem));
    pq->size = 0;
}

static void dijkstraPQPush(DIJKPriorityQueue* pq, int vertex, int dist)
{
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->items = (DIJKPQItem*)realloc(pq->items, (size_t)pq->capacity * sizeof(DIJKPQItem));
    }
    int i = pq->size++;
    pq->items[i].vertex = vertex;
    pq->items[i].dist = dist;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->items[parent].dist <= pq->items[i].dist) {
            break;
        }
        DIJKPQItem temp = pq->items[parent];
        pq->items[parent] = pq->items[i];
        pq->items[i] = temp;
        i = parent;
    }
}

static int dijkstraPQPop(DIJKPriorityQueue* pq, int* vertex, int* dist)
{
    if (pq->size == 0) {
        return 0;
    }
    *vertex = pq->items[0].vertex;
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
            DIJKPQItem temp = pq->items[i];
            pq->items[i] = pq->items[smallest];
            pq->items[smallest] = temp;
            i = smallest;
        }
    }
    return 1;
}

static void dijkstraPQFree(DIJKPriorityQueue* pq)
{
    free(pq->items);
}

DIJKGraph dijkstraGraphCreate(int vertexCount)
{
    DIJKGraph graph;
    graph.vertexCount = vertexCount;
    graph.maxEdges = vertexCount * vertexCount * 2;
    graph.head = (int*)malloc((size_t)vertexCount * sizeof(int));
    for (int i = 0; i < vertexCount; i++) {
        graph.head[i] = -1;
    }
    graph.edges = (DIJKEdge*)malloc((size_t)graph.maxEdges * sizeof(DIJKEdge));
    graph.edgeCount = 0;
    return graph;
}

void dijkstraGraphAddEdge(DIJKGraph* graph, int from, int to, int len)
{
    graph->edges[graph->edgeCount].to = to;
    graph->edges[graph->edgeCount].len = len;
    graph->edges[graph->edgeCount].next = graph->head[from];
    graph->head[from] = graph->edgeCount;
    graph->edgeCount++;
}

void dijkstraGraphAddUndirectedEdge(DIJKGraph* graph, int from, int to, int len)
{
    dijkstraGraphAddEdge(graph, from, to, len);
    dijkstraGraphAddEdge(graph, to, from, len);
}

void dijkstraGraphFree(DIJKGraph* graph)
{
    free(graph->head);
    free(graph->edges);
}

DIJKResult dijkstraCompute(DIJKGraph* graph, int source)
{
    DIJKResult result;
    result.size = graph->vertexCount;
    result.distances = (int*)malloc((size_t)graph->vertexCount * sizeof(int));
    result.visited = (int*)malloc((size_t)graph->vertexCount * sizeof(int));

    for (int i = 0; i < graph->vertexCount; i++) {
        result.distances[i] = INT_MAX;
        result.visited[i] = 0;
    }
    result.distances[source] = 0;

    DIJKPriorityQueue pq;
    dijkstraPQInit(&pq);
    dijkstraPQPush(&pq, source, 0);

    while (pq.size > 0) {
        int vertex, dist;
        dijkstraPQPop(&pq, &vertex, &dist);

        if (dist != result.distances[vertex]) {
            continue;
        }
        result.visited[vertex] = 1;

        for (int e = graph->head[vertex]; e != -1; e = graph->edges[e].next) {
            int neighbor = graph->edges[e].to;
            if (result.visited[neighbor]) {
                continue;
            }
            int newDist = dist + graph->edges[e].len;
            if (newDist < result.distances[neighbor]) {
                result.distances[neighbor] = newDist;
                dijkstraPQPush(&pq, neighbor, newDist);
            }
        }
    }

    dijkstraPQFree(&pq);
    return result;
}

DIJKResult dijkstraComputeMultiSource(DIJKGraph* graph, int* sources, int sourceCount)
{
    DIJKResult result;
    result.size = graph->vertexCount;
    result.distances = (int*)malloc((size_t)graph->vertexCount * sizeof(int));
    result.visited = (int*)malloc((size_t)graph->vertexCount * sizeof(int));

    for (int i = 0; i < graph->vertexCount; i++) {
        result.distances[i] = INT_MAX;
        result.visited[i] = 0;
    }

    for (int i = 0; i < sourceCount; i++) {
        result.distances[sources[i]] = 0;
    }

    DIJKPriorityQueue pq;
    dijkstraPQInit(&pq);
    for (int i = 0; i < sourceCount; i++) {
        dijkstraPQPush(&pq, sources[i], 0);
    }

    while (pq.size > 0) {
        int vertex, dist;
        dijkstraPQPop(&pq, &vertex, &dist);

        if (dist != result.distances[vertex]) {
            continue;
        }
        result.visited[vertex] = 1;

        for (int e = graph->head[vertex]; e != -1; e = graph->edges[e].next) {
            int neighbor = graph->edges[e].to;
            if (result.visited[neighbor]) {
                continue;
            }
            int newDist = dist + graph->edges[e].len;
            if (newDist < result.distances[neighbor]) {
                result.distances[neighbor] = newDist;
                dijkstraPQPush(&pq, neighbor, newDist);
            }
        }
    }

    dijkstraPQFree(&pq);
    return result;
}

void dijkstraResultFree(DIJKResult* result)
{
    free(result->distances);
    free(result->visited);
}