#include "csvPrinter.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    const char* input = "input.csv";
    const char* output = "output.txt";

    if (argc > 1) {
        input = argv[1];
    }
    if (argc > 2) {
        output = argv[2];
    }

    Table table = { 0 };

    if (!readCsv(input, &table)) {
        fprintf(stderr, "Error reading CSV file: %s\n", input);
        return 1;
    }

    if (!writeTable(output, &table)) {
        fprintf(stderr, "Error writing output file: %s\n", output);
        freeTable(&table);
        return 1;
    }

    freeTable(&table);
    return 0;
}
