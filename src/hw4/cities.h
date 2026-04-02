#pragma once
#include "dijkstra.h"
#include <stddef.h>

typedef DIJKGraph Graph;
typedef DIJKResult DIJKResult;

// creates a graph for n cities
Graph createGraph(int n);

// adds a road between two cities
void addEdge(Graph* graph, int from, int to, int len);

// frees all memory associated with the graph
void freeGraph(Graph* graph);

// priority queue structure for state expansion
typedef struct {
    int* itemsDist;
    void* pq;
} StatePQ;

// creates a priority queue for a state
StatePQ createStatePQ(int n, int* stateCities, int cityCount, Graph* graph);

// frees all memory associated with StatePQ
void freeStatePQ(StatePQ* spq);

// finds the nearest unassigned city to the state
int findNearestCity(StatePQ* spq, int n, int* visited);

// updates the state's distances after adding a new city
void updateStatePQ(StatePQ* spq, int newCity, Graph* graph);

// checks if any cities remain unassigned
int anyUnassigned(int n, int* visited);

// dynamic array of city indices
typedef struct {
    int* cities;
    size_t count;
    size_t capacity;
} CityList;

// frees all memory associated with City list
void freeCityList(CityList* list);

// adds a city to the CityList
int addCityToList(CityList* list, int city);

// prints the allocation of cities
void printStateAllocation(int n, int k, CityList* states);