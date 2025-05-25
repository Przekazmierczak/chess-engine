#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
    public:
        Board current_board;

    Game();

    int menu();
};

#endif