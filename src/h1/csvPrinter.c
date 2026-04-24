#include "csvPrinter.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isNumber(const char* str)
{
    if (str == NULL || *str == '\0')
        return false;

    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

void freeTable(Table* table)
{
    if (!table)
        return;

    for (int i = 0; i < table->numCols; i++) {
        free(table->headers[i]);
    }

    for (int i = 0; i < table->numRows; i++) {
        for (int j = 0; j < table->numCols; j++) {
            free(table->data[i][j]);
        }
    }
}

bool readCsv(const char* filename, Table* table)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("file not found!\n");
        return false;
    }

    char buffer[BUFFER_SIZE];
    int row = 0;

    // Чтение заголовков
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        char* token = strtok(buffer, ",");
        table->numCols = 0;

        while (token != NULL && table->numCols < MAX_COLS) {
            table->headers[table->numCols] = malloc(strlen(token) + 1);
            strcpy(table->headers[table->numCols], token);
            table->numCols++;
            token = strtok(NULL, ",");
        }
    }

    // Чтение данных
    while (fgets(buffer, sizeof(buffer), file) != NULL && row < MAX_ROWS) {
        buffer[strcspn(buffer, "\n")] = '\0';

        int col = 0;
        char* start = buffer;
        char* comma;

        while (col < table->numCols) {
            comma = strchr(start, ',');

            if (comma != NULL) {
                int len = comma - start;
                table->data[row][col] = malloc(len + 1);
                strncpy(table->data[row][col], start, len);
                table->data[row][col][len] = '\0';
                start = comma + 1;
            } else {
                table->data[row][col] = malloc(strlen(start) + 1);
                strcpy(table->data[row][col], start);
            }
            col++;
        }

        row++;
    }

    table->numRows = row;
    fclose(file);
    return true;
}

static void calculateColumnWidths(Table* table)
{
    for (int i = 0; i < table->numCols; i++) {
        table->colWidths[i] = strlen(table->headers[i]);
    }

    for (int i = 0; i < table->numRows; i++) {
        for (int j = 0; j < table->numCols; j++) {
            int len = strlen(table->data[i][j]);
            if (len > table->colWidths[j]) {
                table->colWidths[j] = len;
            }
        }
    }
}

static void drawHorizontalLine(FILE* out, Table* table, char separator)
{
    fprintf(out, "+");
    for (int i = 0; i < table->numCols; i++) {
        for (int j = 0; j < table->colWidths[i] + 2; j++) {
            fprintf(out, "%c", separator);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");
}

static void writeRow(FILE* out, Table* table, int row, int isHeader)
{
    fprintf(out, "|");
    for (int i = 0; i < table->numCols; i++) {
        char* cell;
        if (row == -1) {
            cell = table->headers[i];
        } else {
            cell = table->data[row][i];
        }

        int width = table->colWidths[i];

        if (row == -1) {
            fprintf(out, " %-*s |", width, cell);
        } else {
            if (isNumber(cell)) {
                fprintf(out, " %*s |", width, cell);
            } else {
                fprintf(out, " %-*s |", width, cell);
            }
        }
    }
    fprintf(out, "\n");
}

bool writeTable(const char* filename, Table* table)
{
    FILE* out = fopen(filename, "w");
    if (out == NULL) {
        return false;
    }

    calculateColumnWidths(table);

    drawHorizontalLine(out, table, '=');
    writeRow(out, table, -1, 1);
    drawHorizontalLine(out, table, '=');

    for (int i = 0; i < table->numRows; i++) {
        writeRow(out, table, i, 0);
        drawHorizontalLine(out, table, '-');
    }

    fclose(out);
    return true;
}
