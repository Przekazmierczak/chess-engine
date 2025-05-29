#include "Board.h"
#include "Piece.h"
#include "Game.h"

#include "unordered_map"
#include "tuple"

int main() {
    Game& game = Game::get_instance();

    game.menu();

    return 0;
}