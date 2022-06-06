cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctci)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executables
add_executable(99_01_water_divide 99_01_water_divide.cpp)
add_executable(99_02_string_list_dist_1 99_02_string_list_dist_1.cpp)
add_executable(99_03_rumor_spreading 99_03_rumor_spreading.cpp)
add_executable(99_04_in_memory_social_network 99_04_in_memory_social_network.cpp)
add_executable(99_05_operator_maximizer 99_05_operator_maximizer.cpp)
