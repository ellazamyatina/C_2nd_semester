#pragma once
#include <stddef.h>

// edge structure for adjacency list representation
typedef struct {
    int to;
    int len;
    int next;
} DIJKEdge;

// graph structure using adjacency list
typedef struct {
    int vertexCount;
    int maxEdges;
    int* head;
    DIJKEdge* edges;
    int edgeCount;
} DIJKGraph;

// structure containing shortest paths from source
typedef struct {
    int* distances;
    int* visited;
    int size;
} DIJKResult;

// creates a graph with the specified number of vertices
DIJKGraph dijkstraGraphCreate(int vertexCount);

// adds an directed edge to the grap
void dijkstraGraphAddEdge(DIJKGraph* graph, int from, int to, int len);

// adds an undirected edge to the graph
void dijkstraGraphAddUndirectedEdge(DIJKGraph* graph, int from, int to, int len);

// frees all memory associated with the graph
void dijkstraGraphFree(DIJKGraph* graph);

// computes shortest paths from a single source using Dijkstra's algorithm
DIJKResult dijkstraCompute(DIJKGraph* graph, int source);

// computes shortest paths from multiple sources using Dijkstra's algorithm
DIJKResult dijkstraComputeMultiSource(DIJKGraph* graph, int* sources, int sourceCount);

// frees all memory associated with the result
void dijkstraResultFree(DIJKResult* result);
