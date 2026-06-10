#include <iostream>

#include "Board.h"
#include "Game.h"
#include "Piece.h"
#include "Uci.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    Uci uci;
    uci.loop();
    return 0;
}