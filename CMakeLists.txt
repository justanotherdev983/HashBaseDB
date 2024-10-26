cmake_minimum_required(VERSION 3.10)
project(HashBase)

set(CMAKE_CXX_STANDARD 20)


set(SOURCES
    src/core/main.cpp
    src/core/database.cpp
    src/core/hash.cpp
    
)

include_directories(include)


add_executable(hashbase ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

set(CMAKE_C_FLAGS "-Wall -Wextra -g -O2")
