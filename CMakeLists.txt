cmake_minimum_required(VERSION 3.10)
project(HashBase)

set(CMAKE_CXX_STANDARD 20)

# List all source files
set(SOURCES
    src/core/main.cpp
    src/core/database.cpp
    src/core/hash.cpp
    src/frontend/window.cpp
    src/services/db_service.cpp
    src/test/unit_tests.cpp
)

# Include directories
include_directories(include)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL2_ttf using pkg-config
include(FindPkgConfig)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Add executable
add_executable(hashbase ${SOURCES})

# Link libraries
target_link_libraries(hashbase 
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O2")