#include "../csvPrinter.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_is_number()
{
    printf("Testing isNumber...\n");

    assert(isNumber("123") == 1);
    assert(isNumber("abc") == 0);

    printf("isNumber tests passed\n");
}

void test_read_csv()
{
    printf("Testing readCsv...\n");

    Table table = { 0 };
    int result = readCsv("tests/test_files/test1.csv", &table);

    assert(result == 0);
    assert(table.numCols == 3);
    assert(table.numRows == 2);

    assert(strcmp(table.headers[0], "Name") == 0);
    assert(strcmp(table.headers[1], "Age") == 0);
    assert(strcmp(table.headers[2], "Score") == 0);

    assert(strcmp(table.data[0][0], "John") == 0);
    assert(strcmp(table.data[0][1], "25") == 0);
    assert(strcmp(table.data[0][2], "85.5") == 0);
    assert(strcmp(table.data[1][0], "Alice") == 0);
    assert(strcmp(table.data[1][1], "30") == 0);
    assert(strcmp(table.data[1][2], "92.3") == 0);

    freeTable(&table);
    printf("readCsv tests passed\n");
}

void test_write_table()
{
    printf("Testing writeTable...\n");

    Table table = { 0 };
    table.numCols = 2;
    table.numRows = 2;

    table.headers[0] = strdup("Test field 1");
    table.headers[1] = strdup("Test field 2");

    table.data[0][0] = strdup("test");
    table.data[0][1] = strdup("123");
    table.data[1][0] = strdup("long string");
    table.data[1][1] = strdup("28.7");

    int result = writeTable("tests/test_files/output_test.txt", &table);
    assert(result == 0);

    FILE* out = fopen("tests/test_files/output_test.txt", "r");
    assert(out != NULL);
    fclose(out);

    freeTable(&table);
    printf("writeTable tests passed\n");
}

void test_empty_cells()
{
    printf("Testing emptyCells...\n");

    Table table = { 0 };
    int result = readCsv("tests/test_files/test2.csv", &table);

    assert(result == 0);
    assert(table.numCols == 3);
    assert(table.numRows == 2);

    assert(strcmp(table.data[0][0], "data1") == 0);
    assert(strcmp(table.data[0][1], "") == 0);
    assert(strcmp(table.data[0][2], "data3") == 0);
    assert(strcmp(table.data[1][0], "") == 0);
    assert(strcmp(table.data[1][1], "data2") == 0);
    assert(strcmp(table.data[1][2], "") == 0);

    freeTable(&table);
    printf("emptyCells tests passed\n");
}

int main()
{
    printf("Running CSV Printer tests...\n\n");

    test_is_number();
    test_read_csv();
    test_write_table();
    test_empty_cells();

    printf("\nAll tests passed\n");
    return 0;
}
