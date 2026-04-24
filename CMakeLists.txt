cmake_minimum_required(VERSION 3.25)

project(HwC2)

enable_testing()

add_subdirectory(src/h5/)

add_compile_options(-Wall -Wextra -Wpedantic)
