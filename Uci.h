#ifndef CHESSENGINE_UCI_H
#define CHESSENGINE_UCI_H

#include <iostream>
#include <string>
#include <sstream>
#include "Board.h"

class Uci {
public:
    void loop() {
        // WYŁĄCZENIE BUFOROWANIA - KRYTYCZNE DLA SILNIKÓW UCI!
        setvbuf(stdout, NULL, _IONBF, 0);

        Board board;
        bool isWhiteTurn = true;
        std::string line;

        while (std::getline(std::cin, line)) {
            std::stringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "uci") {
                std::cout << "id name JanChess 1.0\n";
                std::cout << "id author Jan Wilomski\n";
                std::cout << "uciok\n";
            }
            else if (token == "isready") {
                std::cout << "readyok\n";
            }
            else if (token == "ucinewgame") {
                board = Board();
                isWhiteTurn = true;
            }
            else if (token == "position") {
                std::string subToken;
                ss >> subToken;

                if (subToken == "startpos") {
                    board = Board();
                    isWhiteTurn = true;
                }

                // Szukamy słowa "moves", bezpiecznie ignorując resztę (np. format FEN)
                while (ss >> subToken) {
                    if (subToken == "moves") break;
                }

                // Odczytujemy same ruchy (tylko te, które mają min. 4 znaki)
                while (ss >> subToken) {
                    if (subToken.length() >= 4) {
                        int startX = subToken[0] - 'a';
                        int startY = subToken[1] - '1';
                        int endX = subToken[2] - 'a';
                        int endY = subToken[3] - '1';

                        Move m(startY * 8 + startX, endY * 8 + endX);
                        board.makeMove(m);
                        isWhiteTurn = !isWhiteTurn;
                    }
                }
            }
            else if (token == "go") {
                Move best = board.getRandomLegalMove(isWhiteTurn);

                // TARCZA 3: Jeśli mamy ruch, wysyłamy go. Jeśli jest Mat - mówimy "0000".
                if (best.startSquare != -1 && best.targetSquare != -1) {
                    std::cout << "bestmove "
                              << board.indexToNotation(best.startSquare)
                              << board.indexToNotation(best.targetSquare)
                              << std::endl; // std::endl wymusza natychmiastowe wysłanie!
                } else {
                    std::cout << "bestmove 0000" << std::endl;
                }
            }
            else if (token == "quit") {
                break;
            }
        }
    }
};

#endif //CHESSENGINE_UCI_H