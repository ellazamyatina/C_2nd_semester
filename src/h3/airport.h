#pragma once
#include "avl.h"

typedef struct {
    int code;      // IATA as a number
    char* name;    // name of the airport
} Airport;

// converts IATA code to an integer 
int IATAToInt(const char* code);

// converts an integer back to a IATA code
void intToIata(int code, char* buffer);

// loads airport data from a file and builds an AVL tree
Node* loadAirports(const char* filename);

// saves all airports from the AVL tree back to the file
void saveAirports(Node* root, const char* filename);

// inserts a new airport into the AVL tree
Node* insertAirport(Node* root, int code, const char* name);

// searches for an airport by IATA code and prints its information
int findAndPrintAirport(Node* root, int code);

// frees all memory allocated for airports and the tree
void freeAirports(Node* root);

// gets the total number of airports in the tree
int getAirportCount(Node* root);