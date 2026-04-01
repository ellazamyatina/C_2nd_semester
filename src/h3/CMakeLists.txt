add_library(airport airport.c avl.c)

add_executable(aerosoft main.c)

target_link_libraries(aerosoft PUBLIC airport)