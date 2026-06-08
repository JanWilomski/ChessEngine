//
// Created by Jan on 08.06.2026.
//

#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H
#include <algorithm>
#include <array>
#include <iostream>


#include "Piece.h"


class Board {
    private:
    std::array<Piece, 64> board{};

    public:
    Board() {
        board={
            Piece::WhiteRook, Piece::WhiteKnight, Piece::WhiteBishop, Piece::WhiteQueen, Piece::WhiteKing, Piece::WhiteBishop, Piece::WhiteKnight, Piece::WhiteRook,
            Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn, Piece::WhitePawn,
            Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None,
            Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None,
            Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None,
            Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None,
            Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn, Piece::BlackPawn,
            Piece::BlackRook, Piece::BlackKnight, Piece::BlackBishop, Piece::BlackQueen, Piece::BlackKing, Piece::BlackBishop, Piece::BlackKnight, Piece::BlackRook
        };
    }

    void printBoard() {
        for (int y = 7; y >= 0; y--) {
            for (int x = 0; x <= 7; x++) {
                std::cout << PieceToString(board[y*8+x]) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::string PieceToString(Piece p) {
        std::string piece;
        switch (p) {
            case Piece::WhiteRook:
                piece = "WR";
                break;
            case Piece::WhiteKnight:
                piece = "WN";
                break;
            case Piece::WhiteBishop:
                piece = "WB";
                break;
            case Piece::WhiteQueen:
                piece = "WQ";
                break;
            case Piece::WhiteKing:
                piece = "WK";
                break;
            case Piece::WhitePawn:
                piece = "WP";
                break;
            case Piece::BlackRook:
                piece = "BR";
                break;
            case Piece::BlackKnight:
                piece = "BN";
                break;
            case Piece::BlackBishop:
                piece = "BB";
                break;
            case Piece::BlackQueen:
                piece = "BQ";
                break;
            case Piece::BlackKing:
                piece = "BK";
                break;
            case Piece::BlackPawn:
                piece = "BP";
                break;
            default:
                piece = "X";
        }
        return piece;
    }

    bool isWhitePiece(Piece p) {
        return (p == Piece::WhiteRook || p == Piece::WhiteBishop || p == Piece::WhiteKnight || p == Piece::WhiteQueen || p == Piece::WhiteKing || p == Piece::WhitePawn);
    }

    void play() {
        std::string ruch;
        while (true) {
            printBoard();
            std::cout << "\nPodaj ruch (np. e2e4) lub wpisz 'exit': ";
            std::cin >> ruch;

            if (ruch == "exit") break;

            // 1. Dekodowanie pola startowego
            int startX = ruch[0] - 'a';
            int startY = ruch[1] - '1';
            int startIndex = startY * 8 + startX;

            // 2. Dekodowanie pola docelowego
            int endX = ruch[2] - 'a';
            int endY = ruch[3] - '1';
            int endIndex = endY * 8 + endX;

            // 3. Wykonanie ruchu na planszy
            board[endIndex] = board[startIndex];
            board[startIndex] = Piece::None;
        }
    }



};


#endif //CHESSENGINE_BOARD_H
