#pragma once

#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_CELL_LENGTH 100
#define BUFFER_SIZE 100

// struct for table srorage
typedef struct {
    char* headers[MAX_COLS];
    char* data[MAX_ROWS][MAX_COLS];
    int colWidths[MAX_COLS];
    int numCols;
    int numRows;
} Table;

// function for reading csv file
int readCsv(const char* filename, Table* table);

// function for writing table as pseudographics
int writeTable(const char* filename, Table* table);

// function for free table
void freeTable(Table* table);

// function for checking row is number
int isNumber(const char* str);
