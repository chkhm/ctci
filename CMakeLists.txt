cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctci)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/01_arrays_and_strings)
add_subdirectory(src/99_my_riddles)
