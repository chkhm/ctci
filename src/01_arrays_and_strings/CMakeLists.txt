cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctci)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executables
add_executable(01_01_is_unique 01_01_is_unique.cpp)
add_executable(01_05_one_away 01_05_one_away.cpp)
add_executable(01_07_rotate_matrix 01_07_rotate_matrix.cpp)
add_executable(01_09_string_rotation 01_09_string_rotation.cpp)

