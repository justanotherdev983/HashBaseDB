set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

cmake_minimum_required(VERSION 3.10)
project(HashBase)

set(CMAKE_CXX_STANDARD 20)

# List all source files
set(SOURCES
    src/*.cpp
    src/core/*.cpp
    src/frontend/*.cpp
    src/services/*.cpp
    src/frontend/components/*.cpp
    src/test/*.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Get all source files
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/core/*.cpp"
        "src/frontend/*.cpp"
        "src/frontend/components/*.cpp"
        "src/services/*.cpp"
        "src/test/*.cpp"
)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -DCMAKE_EXPORT_COMPILE_COMMANDS=1")