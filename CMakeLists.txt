cmake_minimum_required(VERSION 4.2)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 26)

add_executable(ChessEngine main.cpp
        Pieces.cpp
        Pieces.h)
