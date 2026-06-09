#ifndef CHESSENGINE_MOVE_H
#define CHESSENGINE_MOVE_H

struct Move {
    int startSquare;
    int targetSquare;

    Move(int start, int target) {
        startSquare = start;
        targetSquare = target;
    }
};

#endif //CHESSENGINE_MOVE_H