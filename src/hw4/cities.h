#ifndef CITIES_H
#define CITIES_H

#include <stddef.h>

typedef struct {
    int city;
    int dist;
} NearestCity;

typedef struct {
    int* cities;
    size_t count;
    size_t capacity;
} CityList;

void freeCityList(CityList* list);
int addCityToList(CityList* list, int city);
void printStateAllocation(int n, int k, CityList* states);

#endif