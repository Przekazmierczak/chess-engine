#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#include "Types.h"

class Piece;

class Board {
    public:
        const int ROWS = 8;
        const int COLS = 8;
        std::string turn;
        std::string castling;
        std::array<int, 2> enpassant;
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

        PositionSet attacked_positions;
        PositionMap checkin_pieces;
        PositionMap pinned_pieces;

        Board();

        Board(
            std::string input_turn,
            std::string input_castling,
            std::array<std::array<char, 8>, 8> simplify_board
        );

        Board(
            std::string input_turn,
            std::string input_castling,
            std::array<int, 2> input_enpassant,
            std::array<std::array<char, 8>, 8> simplify_board
        );

        bool operator==(const Board& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Board& board_class);

        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board();
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board(std::array<std::array<char, 8>, 8> simplify_board);

        void get_possible_actions();
        
};

#endif