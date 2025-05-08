#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#include "Types.h"

class Piece;

class Board {
    public:
        // Dimensions of the chessboard
        const int ROWS = 8;
        const int COLS = 8;

        // Board state attributes
        std::string turn; // Current player's turn ("white" or "black")
        std::string castling; // Castling rights (e.g., "KQkq")
        std::array<int, 2> enpassant; // Coordinates for en passant, if available

        // 2D array of unique pointers to Piece objects
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

        // Chessboard analysis data
        PositionSet attacked_positions; // Positions attacked by the opponent
        PositionMap checkin_pieces; // Pieces causing a check on the king
        PositionMap pinned_pieces; // Pieces pinned to the king
        PositionSet active_pieces;

        std::string winner;

        // Constructors
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

        // Comparison operators for equality and inequality'
        bool operator==(const Board& other) const;
        bool operator!=(const Board& other) const;

        // Overloaded output stream operator for the board
        friend std::ostream& operator<<(std::ostream& out, const Board& board_class);

        void print_white_perspective();

        // Helper functions for managing the board
        void create_add_piece(
            const char& symbol, // Character representing the piece (e.g., 'K', 'p')
            std::array<std::array<std::unique_ptr<Piece>, 8>, 8>& board, // Board to modify
            const int& row, // Row position
            const int& col // Column position
        );

        // Initialize the board with the standard starting position
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board();

        // Initialize the board with a custom configuration
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board(const std::array<std::array<char, 8>, 8>& simplify_board);

        // Calculate possible moves for the current player
        void get_possible_actions();

        void make_action(int old_row, int old_col, int new_row, int new_col);

        void check_enpassant(int old_row, int old_col, int new_row);

        void check_castling(int old_row, int old_col);

        void computer_action();

        // void print_possible_actions();
};

struct Notation {
    int row;
    char column;

    Notation(std::string input_position);
    Notation(int row, int col);

    std::array<int, 2> parse_square_notation();

    friend std::ostream& operator<<(std::ostream& out, const Notation& notation);
};

#endif