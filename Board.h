#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>

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
        board = {
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
                std::cout << PieceToString(board[y * 8 + x]) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::string PieceToString(Piece p) {
        switch (p) {
            case Piece::WhiteRook: return "WR";
            case Piece::WhiteKnight: return "WN";
            case Piece::WhiteBishop: return "WB";
            case Piece::WhiteQueen: return "WQ";
            case Piece::WhiteKing: return "WK";
            case Piece::WhitePawn: return "WP";
            case Piece::BlackRook: return "BR";
            case Piece::BlackKnight: return "BN";
            case Piece::BlackBishop: return "BB";
            case Piece::BlackQueen: return "BQ";
            case Piece::BlackKing: return "BK";
            case Piece::BlackPawn: return "BP";
            default: return "X ";
        }
    }

    bool isWhitePiece(Piece p) {
        return (p == Piece::WhiteRook || p == Piece::WhiteBishop || p == Piece::WhiteKnight || p == Piece::WhiteQueen || p == Piece::WhiteKing || p == Piece::WhitePawn);
    }
    
    bool isBlackPiece(Piece p) {
        return (p == Piece::BlackRook || p == Piece::BlackBishop || p == Piece::BlackKnight || p == Piece::BlackQueen || p == Piece::BlackKing || p == Piece::BlackPawn);
    }

    Piece getPiece(int index) {
        if (index < 0 || index > 63) return Piece::None;
        return board[index];
    }

    void makeMove(Move m) {
        if (m.startSquare < 0 || m.startSquare > 63 || m.targetSquare < 0 || m.targetSquare > 63) return;
        Piece p = board[m.startSquare];

        bool isEnPassant = (p == Piece::WhitePawn || p == Piece::BlackPawn) && (m.targetSquare == enPassantSquare);

        // Prawa do roszady
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

        // Roszada (przesunięcie wieży)
        if ((p == Piece::WhiteKing || p == Piece::BlackKing) && std::abs(m.startSquare - m.targetSquare) == 2) {
            if (m.targetSquare == 6) { board[5] = Piece::WhiteRook; board[7] = Piece::None; }
            else if (m.targetSquare == 2) { board[3] = Piece::WhiteRook; board[0] = Piece::None; }
            else if (m.targetSquare == 62) { board[61] = Piece::BlackRook; board[63] = Piece::None; }
            else if (m.targetSquare == 58) { board[59] = Piece::BlackRook; board[56] = Piece::None; }
        }

        // Przesunięcie figury
        board[m.targetSquare] = board[m.startSquare];
        board[m.startSquare] = Piece::None;

        // Bicie w przelocie (usunięcie ofiary)
        if (isEnPassant) {
            if (p == Piece::WhitePawn) board[m.targetSquare - 8] = Piece::None;
            else board[m.targetSquare + 8] = Piece::None;
        }

        // Promocja piona
        if (p == Piece::WhitePawn && m.targetSquare / 8 == 7) board[m.targetSquare] = Piece::WhiteQueen;
        else if (p == Piece::BlackPawn && m.targetSquare / 8 == 0) board[m.targetSquare] = Piece::BlackQueen;

        // Reset i ustawienie enPassantSquare
        enPassantSquare = -1;
        if (p == Piece::WhitePawn && m.targetSquare == m.startSquare + 16) enPassantSquare = m.startSquare + 8;
        else if (p == Piece::BlackPawn && m.targetSquare == m.startSquare - 16) enPassantSquare = m.startSquare - 8;
    }

    bool isLegalMove(Move m) {
        if (m.startSquare < 0 || m.startSquare > 63 || m.targetSquare < 0 || m.targetSquare > 63) return false;
        if (m.startSquare == m.targetSquare) return false;

        Piece startPiece = board[m.startSquare];
        Piece targetPiece = board[m.targetSquare];

        if (isWhitePiece(startPiece) && isWhitePiece(targetPiece)) return false;
        if (isBlackPiece(startPiece) && isBlackPiece(targetPiece)) return false;

        int startX = m.startSquare % 8;
        int startY = m.startSquare / 8;
        int endX = m.targetSquare % 8;
        int endY = m.targetSquare / 8;
        int diffX = std::abs(startX - endX);
        int diffY = std::abs(startY - endY);

        if (startPiece == Piece::WhiteKnight || startPiece == Piece::BlackKnight) {
            if ((diffX == 2 && diffY == 1) || (diffX == 1 && diffY == 2)) return true;
        }

        // --- Zabezpieczony Król ---
        if (startPiece == Piece::WhiteKing) {
            if (diffX <= 1 && diffY <= 1) return true;
            if (m.targetSquare == 6 && !wKingMoved && !wRookHMoved && board[7] == Piece::WhiteRook && board[5] == Piece::None && board[6] == Piece::None) {
                if (!isInCheck(true) && !isSquareAttacked(5, false) && !isSquareAttacked(6, false)) return true;
            }
            if (m.targetSquare == 2 && !wKingMoved && !wRookAMoved && board[0] == Piece::WhiteRook && board[1] == Piece::None && board[2] == Piece::None && board[3] == Piece::None) {
                if (!isInCheck(true) && !isSquareAttacked(2, false) && !isSquareAttacked(3, false)) return true;
            }
        }

        if (startPiece == Piece::BlackKing) {
            if (diffX <= 1 && diffY <= 1) return true;
            if (m.targetSquare == 62 && !bKingMoved && !bRookHMoved && board[63] == Piece::BlackRook && board[61] == Piece::None && board[62] == Piece::None) {
                if (!isInCheck(false) && !isSquareAttacked(61, true) && !isSquareAttacked(62, true)) return true;
            }
            if (m.targetSquare == 58 && !bKingMoved && !bRookAMoved && board[56] == Piece::BlackRook && board[57] == Piece::None && board[58] == Piece::None && board[59] == Piece::None) {
                if (!isInCheck(false) && !isSquareAttacked(58, true) && !isSquareAttacked(59, true)) return true;
            }
        }

        // --- Wieża / Hetman (w linii prostej) ---
        if (startPiece == Piece::WhiteRook || startPiece == Piece::BlackRook || startPiece == Piece::WhiteQueen || startPiece == Piece::BlackQueen) {
            if (startX == endX || startY == endY) {
                int step = (startX == endX) ? ((endY > startY) ? 8 : -8) : ((endX > startX) ? 1 : -1);
                int currentSquare = m.startSquare + step;
                while (currentSquare != m.targetSquare) {
                    if (board[currentSquare] != Piece::None) return false;
                    currentSquare += step;
                }
                return true;
            }
        }

        // --- Goniec / Hetman (po skosie) ---
        if (startPiece == Piece::WhiteBishop || startPiece == Piece::BlackBishop || startPiece == Piece::WhiteQueen || startPiece == Piece::BlackQueen) {
            if (diffX == diffY) {
                int step = 0;
                if (startY < endY) step += 8; else step -= 8;
                if (startX < endX) step += 1; else step -= 1;
                int currentSquare = m.startSquare + step;
                while (currentSquare != m.targetSquare) {
                    if (board[currentSquare] != Piece::None) return false;
                    currentSquare += step;
                }
                return true;
            }
        }

        // --- Pionki ---
        if (startPiece == Piece::WhitePawn) {
            if (m.targetSquare == m.startSquare + 8 && targetPiece == Piece::None) return true;
            if (startY == 1 && m.targetSquare == m.startSquare + 16 && targetPiece == Piece::None && board[m.startSquare + 8] == Piece::None) return true;
            if ((m.targetSquare == m.startSquare + 7 || m.targetSquare == m.startSquare + 9) && (targetPiece != Piece::None || m.targetSquare == enPassantSquare)) {
                if (diffX == 1) return true;
            }
        }

        if (startPiece == Piece::BlackPawn) {
            if (m.targetSquare == m.startSquare - 8 && targetPiece == Piece::None) return true;
            if (startY == 6 && m.targetSquare == m.startSquare - 16 && targetPiece == Piece::None && board[m.startSquare - 8] == Piece::None) return true;
            if ((m.targetSquare == m.startSquare - 7 || m.targetSquare == m.startSquare - 9) && (targetPiece != Piece::None || m.targetSquare == enPassantSquare)) {
                if (diffX == 1) return true;
            }
        }

        return false;
    }

    int getKingSquare(bool isWhite) {
        Piece targetKing = isWhite ? Piece::WhiteKing : Piece::BlackKing;
        for (int i = 0; i < 64; i++) {
            if (board[i] == targetKing) return i;
        }
        return -1;
    }

    bool isSquareAttacked(int targetSquare, bool byWhite) {
        if (targetSquare < 0 || targetSquare > 63) return false;
        for (int i = 0; i < 64; i++) {
            Piece p = board[i];
            if (p == Piece::None) continue;
            if (byWhite && !isWhitePiece(p)) continue;
            if (!byWhite && !isBlackPiece(p)) continue;

            // Króle mają własną matematykę (zapobiega pętli rekurencyjnej!)
            if (p == Piece::WhiteKing || p == Piece::BlackKing) {
                int kx = i % 8; int ky = i / 8;
                int tx = targetSquare % 8; int ty = targetSquare / 8;
                if (std::abs(kx - tx) <= 1 && std::abs(ky - ty) <= 1) return true;
                continue; 
            }

            Move hypotheticalMove(i, targetSquare);
            if (isLegalMove(hypotheticalMove)) return true;
        }
        return false;
    }

    bool isInCheck(bool isWhiteTurn) {
        int kingSquare = getKingSquare(isWhiteTurn);
        if (kingSquare == -1) return false;
        return isSquareAttacked(kingSquare, !isWhiteTurn);
    }

    bool hasLegalMoves(bool isWhiteTurn) {
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
                    if (!temp.isInCheck(isWhiteTurn)) return true;
                }
            }
        }
        return false;
    }

    std::string indexToNotation(int index) {
        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);
        return std::string{file, rank};
    }

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

        if (!legalMoves.empty()) {
            int randomIndex = std::rand() % legalMoves.size();
            return legalMoves[randomIndex];
        }
        return Move(-1, -1);
    }


    int getScore(bool isWhiteTurn) {
        int whiteMaterial = 0;
        int blackMaterial = 0;
        std::map<int, bool> whiteLegalMoves = {};
        std::map<int, bool> blackLegalMoves = {};

        for (int i = 0; i < 64; i++) {
            Piece p = board[i];
            if (p == Piece::None) continue;
            if (p == Piece::WhitePawn)   whiteMaterial += 100;
            else if (p == Piece::WhiteKnight) whiteMaterial += 300;
            else if (p == Piece::WhiteBishop) whiteMaterial += 300;
            else if (p == Piece::WhiteRook)   whiteMaterial += 500;
            else if (p == Piece::WhiteQueen)  whiteMaterial += 900;

            else if (p == Piece::BlackPawn)   blackMaterial += 100;
            else if (p == Piece::BlackKnight) blackMaterial += 300;
            else if (p == Piece::BlackBishop) blackMaterial += 300;
            else if (p == Piece::BlackRook)   blackMaterial += 500;
            else if (p == Piece::BlackQueen)  blackMaterial += 900;

            else if (p == Piece::WhiteKing)   whiteMaterial += 20000;
            else if (p == Piece::BlackKing)   blackMaterial += 20000;

            for (int j = 0; j < 64; j++) {
                if (isWhitePiece(p)&&isLegalMove(Move(i, j))) {
                    whiteLegalMoves.insert({j, true});
                }else if (isBlackPiece(p)&&isLegalMove(Move(i, j)))blackLegalMoves.insert({j, true});
            }
        }
        whiteMaterial += whiteLegalMoves.size()*10;
        blackMaterial += blackLegalMoves.size()*10;


        if (isWhiteTurn) {
            return whiteMaterial - blackMaterial;
        } else {
            return blackMaterial - whiteMaterial;
        }
    }

    Move getBestMove(bool isWhiteTurn, int depth = 4) {
        std::vector<Move> legalMoves = getAllLegalMoves(isWhiteTurn);

        if (legalMoves.empty()) return Move(-1, -1);
        int bestScore = -100000;
        Move bestMove = legalMoves[0];

        for (Move ruch : legalMoves) {
            Board temp = *this;
            temp.makeMove(ruch);

            int score = -temp.negamax(depth - 1, -999999, 999999, !isWhiteTurn);

            // std::cout << "info string Bada ruch: "
            //           << indexToNotation(ruch.startSquare) << indexToNotation(ruch.targetSquare)
            //           << " | Przewidywana ocena: " << score << std::endl;

            if (score > bestScore) {
                bestScore = score;
                bestMove = ruch;
            }
        }
        return bestMove;
    }

    std::vector<Move> getAllLegalMoves(bool isWhiteTurn) {
        std::vector<Move> legalMoves;
        for (int start = 0; start < 64; start++) {
            Piece p = board[start];
            if (p == Piece::None) continue;
            if (isWhiteTurn && !isWhitePiece(p)) continue;
            if (!isWhiteTurn && !isBlackPiece(p)) continue;
            for (int i = 0; i < 64; i++) {
                Move testMove(start, i);
                if (isLegalMove(testMove)) {
                    legalMoves.push_back(testMove);
                }
            }
        }

        return legalMoves;
    }

    int negamax(int depth, int alpha, int beta, bool isWhiteTurn) {
        if (depth <= 0) return getScore(isWhiteTurn);

        std::vector<Move> ruchy = getAllLegalMoves(isWhiteTurn);
        if (ruchy.empty()) {
            if (isInCheck(isWhiteTurn)) return -99999;
            else return 0;
        }

        int bestScore = -999999;

        for (Move ruch : ruchy) {
            Board temp = *this;
            temp.makeMove(ruch);

            int score = -temp.negamax(depth - 1, -beta, -alpha, !isWhiteTurn);

            if (score > bestScore) bestScore = score;
            if (score > alpha) alpha = score;

            if (alpha >= beta) {
                break;
            }
        }
        return bestScore;
    }
};

#endif //CHESSENGINE_BOARD_H