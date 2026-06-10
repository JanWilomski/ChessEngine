cmake_minimum_required(VERSION 4.2)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 26)

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
