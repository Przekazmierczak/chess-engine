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
            const std::string& input_turn,
            const std::string& input_castling,
            const std::array<std::array<char, 8>, 8>& simplify_board
        );

        Board(
            const std::string& input_turn,
            const std::string& input_castling,
            const std::array<int, 2>& input_enpassant,
            const std::array<std::array<char, 8>, 8>& simplify_board
        );

        bool operator==(const Board& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Board& board_class);

        void Board::create_add_piece(const char& symbol, std::array<std::array<std::unique_ptr<Piece>, 8>, 8>& board, const int& row, const int& col);

        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board();
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board(const std::array<std::array<char, 8>, 8>& simplify_board);

        void get_possible_actions();
        
};

#endif