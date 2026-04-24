cmake_minimum_required(VERSION 3.25)
project(aerosoft)

add_executable(aerosoft main.c avl.c)

add_executable(test_avl test_avl.c avl.c)

enable_testing()
add_test(NAME avl_tests COMMAND test_avl)