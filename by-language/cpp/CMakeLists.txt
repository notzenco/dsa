cmake_minimum_required(VERSION 3.16)
project(dsa-cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Data structures library
file(GLOB DS_SOURCES "data-structures/*.cpp")
file(GLOB DS_HEADERS "data-structures/*.hpp")

if(DS_SOURCES)
    add_library(data_structures ${DS_SOURCES})
endif()

# Algorithms library
file(GLOB ALG_SOURCES "algorithms/*.cpp")
file(GLOB ALG_HEADERS "algorithms/*.hpp")

if(ALG_SOURCES)
    add_library(algorithms ${ALG_SOURCES})
endif()

# Testing
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    if(TARGET data_structures)
        target_link_libraries(${TEST_NAME} data_structures)
    endif()

    if(TARGET algorithms)
        target_link_libraries(${TEST_NAME} algorithms)
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
