#Setup testing
enable_testing()

find_package(Check)
if(NOT CHECK_FOUND)
    message(FATAL_ERROR "ERROR: Check not found for tests, failing")
endif(NOT CHECK_FOUND)

# Set some variables
set(TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_INCLUDES ${CHECK_INCLUDE_DIR} ${BMP_LIB_INCLUDE})
set(TEST_LIBS ${CHECK_LIBRARIES} ${BMP_LIB})

# Start adding tests
include_directories(${TEST_INCLUDES})

add_executable(bmpLibTest "${TEST_SRC}/bmpLibTest.c")
target_link_libraries(bmpLibTest ${TEST_LIBS})
add_test(bmpLibTest bmpLibTest)
