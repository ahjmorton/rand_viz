cmake_minimum_required(VERSION 2.8.8)

project(rand_viz C)

# Set some variables
set(CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING "Flags for profiling builds")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(SRC_PATH "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/test")


#Declare BMP object library 
set(BMP_LIB_SRC "${SRC_PATH}/bmpLib")
file(GLOB BMP_LIB_SOURCES "${BMP_LIB_SRC}/*.c")
include_directories("${BMP_LIB_SRC}")
add_library(bmpLib OBJECT ${BMP_LIB_SOURCES})

#Setup testing
enable_testing()

find_package(Check)
if(NOT CHECK_FOUND)
    message(FATAL_ERROR "ERROR: Check not found for tests, failing")
endif(NOT CHECK_FOUND)
