cmake_minimum_required(VERSION 3.0.0)
set(C_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build/bin>)

project(aks)

add_executable(aks src/main.c)