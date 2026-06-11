cmake_minimum_required(VERSION 4.2)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(ChessEngine main.cpp
        Piece.cpp
        Piece.h
        Board.cpp
        Board.h
        Game.cpp
        Game.h
        Move.cpp
        Move.h
        Uci.cpp
        Uci.h)
