cmake_minimum_required(VERSION 3.10)


project(HashBaseDB)


set(CMAKE_C_STANDARD 20)

add_executable(hashbase src/main.cpp)


include_directories(include)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Compiler flags (optional)
# This is where you can add optimization flags, warning levels, etc.
# For example:
set(CMAKE_C_FLAGS "-Wall -Wextra -g -O2")
