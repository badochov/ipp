cmake_minimum_required(VERSION 3.15)
project(vector C)

set(CMAKE_C_STANDARD 11)

add_executable(vector main.c vector.c vector.h)