#ifndef ALFABETA_H
#define ALFABETA_H

#include "Board.h"

class Board;

class AlfaBetaPruning {
public:
    int operator()(Board board, const int depth, int alpha, int beta);
};

#endif