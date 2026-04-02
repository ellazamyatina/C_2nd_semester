add_executable(cities main.c cities.c dijkstra.c)

add_executable(test_cities test_cities.c cities.c dijkstra.c)

add_test(NAME test_cities COMMAND test_cities)
