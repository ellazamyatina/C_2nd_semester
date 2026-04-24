add_library(csvPrinter csvPrinter.c)

add_executable(csv_printer main.c)
target_link_libraries(csv_printer PUBLIC csvPrinter)

add_executable(test_csv_printer tests/test_csv_printer.c)
target_include_directories(test_csv_printer PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_csv_printer PUBLIC csvPrinter)

add_custom_target(run_tests
    COMMAND test_csv_printer
    DEPENDS test_csv_printer
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running tests..."
)

add_dependencies(csv_printer run_tests)
