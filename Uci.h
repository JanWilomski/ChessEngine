#ifndef CHESSENGINE_UCI_H
#define CHESSENGINE_UCI_H

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>    // <--- DODANE (do odczytu zegara systemowego)
#include <cstdlib>  // <--- DODANE (do obsługi rand i srand)
#include "Board.h"

class Uci {
public:
    void loop() {
        setvbuf(stdout, NULL, _IONBF, 0);

        // --- TUTAJ NAPRAWA NR 2 (Losowość) ---
        // Karmimy generator liczb pseudolosowych obecnym czasem z zegara komputera.
        // Dzięki temu silnik w każdej grze zagra coś innego!
        std::srand(std::time(nullptr));

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

                // --- TUTAJ NAPRAWA NR 1 (Reset planszy) ---
                // Niezależnie od tego, czy GUI mówi "startpos", czy daje długi format "fen",
                // ZAWSZE resetujemy planszę przed wczytaniem nowej historii ruchów!
                board = Board();
                isWhiteTurn = true;

                std::string subToken;

                // Przeskakujemy słowa aż nie trafimy na "moves" (ignorujemy całą resztę śmieci z GUI)
                while (ss >> subToken) {
                    if (subToken == "moves") break;
                }

                // Odczytujemy same ruchy i aktualizujemy naszą świeżą planszę
                while (ss >> subToken) {
                    if (subToken.length() >= 4) {
                        int startX = subToken[0] - 'a';
                        int startY = subToken[1] - '1';
                        int endX = subToken[2] - 'a';
                        int endY = subToken[3] - '1';

                        Move m(startY * 8 + startX, endY * 8 + endX);
                        board.makeMove(m);
                        std::cout << "info string Score: " << board.getScore(isWhiteTurn) << std::endl;

                        isWhiteTurn = !isWhiteTurn;
                    }
                }
            }
            else if (token == "go") {
                Move best = board.getBestMove(isWhiteTurn, 5);

                if (best.startSquare != -1 && best.targetSquare != -1) {
                    // Sklejamy standardowy ruch (np. "b2a1")
                    std::string moveString = board.indexToNotation(best.startSquare) + board.indexToNotation(best.targetSquare);

                    // --- NAPRAWA: Zabezpieczenie Promocji dla UCI ---
                    Piece p = board.getPiece(best.startSquare);
                    if (p == Piece::WhitePawn && best.targetSquare / 8 == 7) {
                        moveString += "q"; // Doklejamy 'q' dla białych
                    } else if (p == Piece::BlackPawn && best.targetSquare / 8 == 0) {
                        moveString += "q"; // Doklejamy 'q' dla czarnych
                    }

                    // Wysyłamy poprawiony tekst do GUI
                    std::cout << "bestmove " << moveString << std::endl;

                } else {
                    std::cout << "bestmove 0000" << std::endl;
                }
            }
            else if (token == "eval") {
                int score = board.getScore(isWhiteTurn);
                std::cout << "info string Score: " << score << std::endl;
            }
            else if (token == "quit") {
                break;
            }
        }
    }
};

#endif //CHESSENGINE_UCI_H