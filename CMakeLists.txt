cmake_minimum_required(VERSION 2.6)

project(rand_viz C)

# Set some variables
set(CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING "Flags for profiling builds")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(SRC_PATH "${CMAKE_SOURCE_DIR}/src")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/test")

#Setup testing
enable_testing()

find_package(Check)
if(NOT CHECK_FOUND)
    message(FATAL_ERROR "ERROR: Check not found for tests, failing")
endif(NOT CHECK_FOUND)
