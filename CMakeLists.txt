cmake_minimum_required(VERSION 2.8.8)

project(rand_viz C)

# Set some variables
set(CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING "Flags for profiling builds")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(SRC_PATH "${CMAKE_SOURCE_DIR}/src")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/test")

#Declare BMP variables 
set(BMP_LIB "bmpLib")
set(BMP_LIB_SRC "${SRC_PATH}/bmpLib")
set(BMP_LIB_INCLUDES "${BMP_LIB_SRC}")
file(GLOB BMP_LIB_SOURCES "${BMP_LIB_SRC}/*.c")

#Declare BMP variables 
include_directories("${BMP_LIB_INCLUDES}")
add_library(${BMP_LIB} STATIC ${BMP_LIB_SOURCES})

#Setup testing
enable_testing()
add_subdirectory(test)
