//
// Created by Jan on 08.06.2026.
//

#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>


#include "Move.h"
#include "Piece.h"


class Board {
    private:
    std::array<Piece, 64> board{};
    int enPassantSquare = -1;
    bool wKingMoved = false;
    bool wRookAMoved = false;
    bool wRookHMoved = false;
    bool bKingMoved = false;
    bool bRookAMoved = false;
    bool bRookHMoved = false;


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
    bool isBlackPiece(Piece p) {
        return (p == Piece::BlackRook || p == Piece::BlackBishop || p == Piece::BlackKnight || p == Piece::BlackQueen || p == Piece::BlackKing || p == Piece::BlackPawn);
    }

    std::array<Piece, 64> getBoard() {
        return board;
    }

    void makeMove(Move m) {
        Piece p = board[m.startSquare];

        // 1. Wykrywamy, czy ten ruch to właśnie bicie w przelocie
        bool isEnPassant = (p == Piece::WhitePawn || p == Piece::BlackPawn) && (m.targetSquare == enPassantSquare);

        if (p == Piece::WhiteKing) wKingMoved = true;
        if (p == Piece::BlackKing) bKingMoved = true;
        if (p == Piece::WhiteRook) {
            if (m.startSquare == 0) wRookAMoved = true;
            if (m.startSquare == 7) wRookHMoved = true;
        }
        if (p == Piece::BlackRook) {
            if (m.startSquare == 56) bRookAMoved = true;
            if (m.startSquare == 63) bRookHMoved = true;
        }

        // --- WYKRYWANIE ROSZADY I PRZESUWANIE WIEŻY ---
        if ((p == Piece::WhiteKing || p == Piece::BlackKing) && std::abs(m.startSquare - m.targetSquare) == 2) {
            if (m.targetSquare == 6) { board[5] = Piece::WhiteRook; board[7] = Piece::None; } // Biała krótka
            else if (m.targetSquare == 2) { board[3] = Piece::WhiteRook; board[0] = Piece::None; } // Biała długa
            else if (m.targetSquare == 62) { board[61] = Piece::BlackRook; board[63] = Piece::None; } // Czarna krótka
            else if (m.targetSquare == 58) { board[59] = Piece::BlackRook; board[56] = Piece::None; } // Czarna długa
        }

        // 2. Standardowe przesunięcie figury
        board[m.targetSquare] = board[m.startSquare];
        board[m.startSquare] = Piece::None;


        if (isEnPassant) {
            if (p == Piece::WhitePawn) {
                board[m.targetSquare - 8] = Piece::None;
            } else {
                board[m.targetSquare + 8] = Piece::None;
            }
        }

        if (p == Piece::WhitePawn && m.targetSquare / 8 == 7) {
            board[m.targetSquare] = Piece::WhiteQueen;
        } else if (p == Piece::BlackPawn && m.targetSquare / 8 == 0) {
            board[m.targetSquare] = Piece::BlackQueen;
        }

        enPassantSquare = -1;

        // Jeśli właśnie wykonano podwójny ruch pionem, zapisujemy pole "za nim"
        if (p == Piece::WhitePawn && m.targetSquare == m.startSquare + 16) {
            enPassantSquare = m.startSquare + 8;
        }
        else if (p == Piece::BlackPawn && m.targetSquare == m.startSquare - 16) {
            enPassantSquare = m.startSquare - 8;
        }
    }

    Piece getPiece(int index) {
        return board[index];
    }


    bool isLegalMove(Move m) {
        if (m.startSquare < 0 || m.startSquare > 63 || m.targetSquare < 0 || m.targetSquare > 63) return false;
        if (m.startSquare == m.targetSquare) return false;
        Piece startPiece = board[m.startSquare];
        Piece targetPiece = board[m.targetSquare];

        if (isWhitePiece(startPiece) && isWhitePiece(targetPiece)) return false;
        if (isBlackPiece(startPiece) && isBlackPiece(targetPiece)) return false;

        // Odtwarzamy współrzędne X i Y ze spłaszczonego indeksu (odwrotność naszego wzoru)
        int startX = m.startSquare % 8;
        int startY = m.startSquare / 8;
        int endX = m.targetSquare % 8;
        int endY = m.targetSquare / 8;

        // Różnica (dystans) w osi X i Y
        int diffX = std::abs(startX - endX);
        int diffY = std::abs(startY - endY);

        if (startPiece == Piece::WhiteKnight || startPiece == Piece::BlackKnight) {
            if ((diffX==2&&diffY==1)||(diffX==1&&diffY==2)) {
                return true;
            }
        }
        if (startPiece == Piece::WhiteKing || startPiece == Piece::BlackKing) {
            if (diffX <= 1 && diffY <= 1 && !isSquareAttacked(m.targetSquare, true)) {
                return true;
            }
        }
        if (startPiece == Piece::WhiteKing) {
            if (diffX <= 1 && diffY <= 1) return true; // Zwykły ruch

            // Roszada krótka (Kingside) - najpierw sprawdzamy cel i figury, DOPIERO POTEM szachy
            if (m.targetSquare == 6 && !wKingMoved && !wRookHMoved && board[7] == Piece::WhiteRook && board[5] == Piece::None && board[6] == Piece::None) {
                if (!isInCheck(true) && !isSquareAttacked(5, false) && !isSquareAttacked(6, false)) return true;
            }
            // Roszada długa (Queenside)
            if (m.targetSquare == 2 && !wKingMoved && !wRookAMoved && board[0] == Piece::WhiteRook && board[1] == Piece::None && board[2] == Piece::None && board[3] == Piece::None) {
                if (!isInCheck(true) && !isSquareAttacked(2, false) && !isSquareAttacked(3, false)) return true;
            }
        }

        // Logika Czarnego Króla
        if (startPiece == Piece::BlackKing) {
            if (diffX <= 1 && diffY <= 1) return true; // Zwykły ruch

            // Roszada krótka
            if (m.targetSquare == 62 && !bKingMoved && !bRookHMoved && board[63] == Piece::BlackRook && board[61] == Piece::None && board[62] == Piece::None) {
                if (!isInCheck(false) && !isSquareAttacked(61, true) && !isSquareAttacked(62, true)) return true;
            }
            // Roszada długa
            if (m.targetSquare == 58 && !bKingMoved && !bRookAMoved && board[56] == Piece::BlackRook && board[57] == Piece::None && board[58] == Piece::None && board[59] == Piece::None) {
                if (!isInCheck(false) && !isSquareAttacked(58, true) && !isSquareAttacked(59, true)) return true;
            }
        }
        if (startPiece == Piece::BlackBishop || startPiece == Piece::WhiteBishop) {
            if (diffX == diffY) {
                int step = 0;
                if (startY<endY) {
                    step+=8;
                }else step-=8;
                if (startX<endX) {
                    step+=1;
                }else step-=1;
                int currentSquare = m.startSquare + step;
                while (currentSquare != m.targetSquare) {
                    if (board[currentSquare] != Piece::None) {
                        return false;
                    }
                    currentSquare += step;
                }
                return true;
            }
        }
        if (startPiece == Piece::BlackQueen || startPiece == Piece::WhiteQueen) {
            if (startX == endX || startY == endY) {
                int step = 0;
                if (startX == endX) {
                    step = (endY > startY) ? 8 : -8;
                } else {
                    step = (endX > startX) ? 1 : -1;
                }
                int currentSquare = m.startSquare + step;
                while (currentSquare != m.targetSquare) {
                    if (board[currentSquare] != Piece::None) {
                        return false;
                    }
                    currentSquare += step;
                }
                return true;
            }
            if (diffX == diffY) {
                int step = 0;
                if (startY<endY) {
                    step+=8;
                }else step-=8;
                if (startX<endX) {
                    step+=1;
                }else step-=1;
                int currentSquare = m.startSquare + step;
                while (currentSquare != m.targetSquare) {
                    if (board[currentSquare] != Piece::None) {
                        return false;
                    }
                    currentSquare += step;
                }
                return true;
            }
        }
        if (startPiece == Piece::WhitePawn) {
            if (m.targetSquare == m.startSquare + 8 && targetPiece == Piece::None) {
                return true;
            }
            if (startY == 1 && m.targetSquare == m.startSquare + 16 && targetPiece == Piece::None && board[m.startSquare + 8] == Piece::None) {
                enPassantSquare=m.startSquare+8;
                return true;
            }

            // Bicie po skosie (+7 lub +9) - cel nie może być pusty i sprawdzamy krawędzie
            if ((m.targetSquare == m.startSquare + 7 || m.targetSquare == m.startSquare + 9) && (targetPiece != Piece::None||m.targetSquare==enPassantSquare)) {
                // Zabezpieczenie przed "zawijaniem" planszy (które omawialiśmy wcześniej)
                if (diffX == 1) {
                    return true;
                }
            }

        }
        if (startPiece == Piece::BlackPawn) {
            // Czarne idą w dół planszy (indeks maleje)

            // Zwykły ruch do przodu o 1 pole
            if (m.targetSquare == m.startSquare - 8 && targetPiece == Piece::None) {
                return true;
            }

            // Podwójny ruch z pozycji startowej (wiersz 7, czyli y == 6)
            if (startY == 6 && m.targetSquare == m.startSquare - 16 && targetPiece == Piece::None && board[m.startSquare - 8] == Piece::None) {
                enPassantSquare=m.startSquare-8;
                return true;
            }

            // Bicie po skosie (-7 lub -9)
            if ((m.targetSquare == m.startSquare - 7 || m.targetSquare == m.startSquare - 9) && (targetPiece != Piece::None||m.targetSquare==enPassantSquare)) {
                if (diffX == 1) {
                    return true;
                }
            }
        }
        return false;
    }

    // Sprawdza, czy gracz ma jakikolwiek legalny i bezpieczny dla Króla ruch
    bool hasLegalMoves(bool isWhiteTurn) {
        for (int start = 0; start < 64; start++) {
            Piece p = board[start];
            if (p == Piece::None) continue;
            if (isWhiteTurn && !isWhitePiece(p)) continue;
            if (!isWhiteTurn && !isBlackPiece(p)) continue;

            // Sprawdzamy każdy możliwy cel dla tej figury
            for (int target = 0; target < 64; target++) {
                Move testMove(start, target);

                // Jeśli kształt ruchu jest legalny...
                if (isLegalMove(testMove)) {
                    // Tworzymy kopię planszy, by sprawdzić, czy Król nie wpadnie w szacha
                    Board temp = *this;
                    temp.makeMove(testMove);
                    if (!temp.isInCheck(isWhiteTurn)) {
                        return true; // Znaleźliśmy co najmniej 1 dozwolony ruch!
                    }
                }
            }
        }
        return false;
    }




    // 1. Szukamy naszego Króla
    int getKingSquare(bool isWhite) {
        Piece targetKing = isWhite ? Piece::WhiteKing : Piece::BlackKing;
        for (int i = 0; i < 64; i++) {
            if (board[i] == targetKing) return i;
        }
        return -1; // Zabezpieczenie, choć Król zawsze powinien być na planszy
    }

    // 2. Sprawdzamy, czy dane pole jest pod ostrzałem
    bool isSquareAttacked(int targetSquare, bool byWhite) {
        if (targetSquare < 0 || targetSquare > 63) return false;
        for (int i = 0; i < 64; i++) {
            Piece p = board[i];
            if (p == Piece::None) continue;


            if (byWhite && !isWhitePiece(p)) continue;
            if (!byWhite && !isBlackPiece(p)) continue;


            Move hypotheticalMove(i, targetSquare);

            if (isLegalMove(hypotheticalMove)) {
                return true;
            }
        }
        return false;
    }


    bool isInCheck(bool isWhiteTurn) {
        int kingSquare = getKingSquare(isWhiteTurn);

        return isSquareAttacked(kingSquare, !isWhiteTurn);
    }


    std::string indexToNotation(int index) {
        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);
        return std::string{file, rank};
    }

    // Zbiera wszystkie ruchy i zwraca jeden z nich
    Move getRandomLegalMove(bool isWhiteTurn) {
        std::vector<Move> legalMoves;

        for (int start = 0; start < 64; start++) {
            Piece p = board[start];
            if (p == Piece::None) continue;
            if (isWhiteTurn && !isWhitePiece(p)) continue;
            if (!isWhiteTurn && !isBlackPiece(p)) continue;

            for (int target = 0; target < 64; target++) {
                Move testMove(start, target);
                if (isLegalMove(testMove)) {
                    Board temp = *this;
                    temp.makeMove(testMove);
                    if (!temp.isInCheck(isWhiteTurn)) {
                        legalMoves.push_back(testMove);
                    }
                }
            }
        }

        // Jeśli są jakieś ruchy, zwracamy losowy
        if (!legalMoves.empty()) {
            int randomIndex = std::rand() % legalMoves.size();
            return legalMoves[randomIndex];
        }

        // Zabezpieczenie (jeśli jest mat/pat)
        return Move(-1, -1);
    }



};


#endif //CHESSENGINE_BOARD_H
