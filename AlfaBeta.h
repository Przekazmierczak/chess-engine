#ifndef ALFABETA_H
#define ALFABETA_H

#include "Board.h"

class Board;

class AlfaBetaPruning {
public:
    // Evaluates the best move for the given board state using the Alpha-Beta pruning algorithm.
    int operator()(Board board, int depth, int alpha, int beta);
};

#endif