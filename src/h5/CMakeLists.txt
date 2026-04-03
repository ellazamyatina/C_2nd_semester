add_executable(h5 main.c dfa.c)
add_executable(test_dfa test_dfa.c dfa.c)
add_test(NAME test_dfa COMMAND test_dfa)
