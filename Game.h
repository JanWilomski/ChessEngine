//
// Created by Jan on 09.06.2026.
//

#ifndef CHESSENGINE_GAME_H
#define CHESSENGINE_GAME_H
#include <array>
#include <iostream>

#include "Board.h"
#include "Piece.h"


class Game {
    private:
    enum MOVE {
        WHITE,
        BLACK,
    };
    MOVE move = WHITE;
    Board board;

    public:

    Game(Board board) {
        this->board = board;
    }

    void play() {
        std::string ruch;
        while (true) {
            board.printBoard();
            std::cout << "\nPodaj ruch (np. e2e4) lub wpisz 'exit': ";
            std::cin >> ruch;

            if (ruch == "exit") break;


            int startX = ruch[0] - 'a';
            int startY = ruch[1] - '1';
            int startIndex = startY * 8 + startX;

            Piece startPiece = board.getPiece(startIndex);

            if (startPiece == Piece::None) {
                std::cout << "Wybrales puste pole! Sprobuj ponownie.\n";
                continue;
            }

            if (move == WHITE && board.isBlackPiece(startPiece)) {
                std::cout << "Teraz ruch bialych!\n";
                continue;
            }
            if (move == BLACK && board.isWhitePiece(startPiece)) {
                std::cout << "Teraz ruch czarnych!\n";
                continue;
            }


            int endX = ruch[2] - 'a';
            int endY = ruch[3] - '1';
            int endIndex = endY * 8 + endX;

            Move m(startIndex, endIndex);

            if (!board.isLegalMove(m)) {
                std::cout << "Nielegalny ruch dla tej figury!\n";
                continue;
            }

            // 3. Wykonanie ruchu na planszy
            board.makeMove(m);

            if (move == WHITE) {
                move = BLACK;
            } else {
                move = WHITE;
            }
        }
    }
};


#endif //CHESSENGINE_GAME_H
