cmake_minimum_required(VERSION 3.21)
project(PRG2_LABO01)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS="-Wall -Wextra -Wconversion -Wsign-conversion -pedantic")

add_executable(DEBUG main.c)