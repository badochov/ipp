cmake_minimum_required(VERSION 3.15)
project(lasy C)

set(CMAKE_C_STANDARD 11)

add_executable(lasy main.c BST.c BST.h forests.c forests.h)