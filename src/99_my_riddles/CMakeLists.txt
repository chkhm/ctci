cmake_minimum_required(VERSION 3.10)

# set the project name
project(ctci)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(_VCPKG_INSTALLED_DIR D:/git/vcpkg/installed/x64-windows)

set(BOOST_ROOT ${_VCPKG_INSTALLED_DIR})
set(Boost_DIR ${_VCPKG_INSTALLED_DIR})
set(Boost_NO_SYSTEM_PATHS ON)

find_package(Boost 1.79.0 ) # COMPONENTS lexical_cast PATHS D:/git/vcpkg/packages) # ${_VCPKG_INSTALLED_DIR})
#if (Boost_FOUND)
	include_directories( ${Boost_INCLUDE_DIRS} )
	add_executable(99_04_in_memory_social_network 99_04_in_memory_social_network.cpp)
#endif()

# add the executables
#add_executable(99_01_water_divide 99_01_water_divide.cpp)
#add_executable(99_02_string_list_dist_1 99_02_string_list_dist_1.cpp)
#add_executable(99_03_rumor_spreading 99_03_rumor_spreading.cpp)
#add_executable(99_04_in_memory_social_network 99_04_in_memory_social_network.cpp)
#target_link_libraries(99_04_in_memory_social_network Boost::lexical_cast)

#add_executable(99_05_operator_maximizer 99_05_operator_maximizer.cpp)
