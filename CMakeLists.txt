cmake_minimum_required(VERSION 3.10)
project(HashBase)

set(CMAKE_CXX_STANDARD 20)


set(SOURCES
    src/core/main.cpp
    src/core/database.cpp
    src/core/hash.cpp
    src/frontend/window.cpp
    src/services/db_service.cpp
    src/test/unit_tests.cpp

)
include_directories(include)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


add_executable(hashbase ${SOURCES})


target_link_libraries(hashbase ${SDL2_LIBRARIES})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)


set(CMAKE_C_FLAGS "-Wall -Wextra -g -O2")
