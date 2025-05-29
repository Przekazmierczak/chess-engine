#include "Board.h"
#include "Piece.h"
#include "Game.h"

#include "unordered_map"
#include "tuple"

int main() {
    Game& game = Game::getInstance();

    game.menu();

    return 0;
}