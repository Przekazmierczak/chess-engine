#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>
#include <algorithm>
#include <functional>

#include "Types.h"
#include "Piece.h"

class Piece;
class Game;

class Board {
    public:
        // Dimensions of the chessboard
        const int ROWS = 8;
        const int COLS = 8;

        // Board state attributes
        PlayerColor turn; // Current player's turn (white = 0 or black = 1)
        std::string castling; // Castling rights (e.g., "KQkq")
        std::array<int, 2> enpassant; // Coordinates for en passant, if available

        // 2D array of unique pointers to Piece objects
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board;

        // Chessboard analysis data
        PositionSet attacked_positions; // Positions attacked by the opponent
        PositionMap checkin_pieces; // Pieces causing a check on the king
        PositionMap pinned_pieces; // Pieces pinned to the king
        PositionSet active_pieces;

        // Board ratings
        const int material_rating_weight = 50;
        const int attack_rating_weight = 3;
        const int protecting_rating_weight = 2;

        int white_material_rating;
        int black_material_rating;
        int white_attack_rating;
        int black_attack_rating;

        int final_rating;

        Winner winner;

        // Constructors
        Board();
        Board(
            const PlayerColor& input_turn,
            const std::string& input_castling,
            const std::array<std::array<char, 8>, 8>& simplify_board
        );
        Board(
            const PlayerColor& input_turn,
            const std::string& input_castling,
            const std::array<int, 2>& input_enpassant,
            const std::array<std::array<char, 8>, 8>& simplify_board
        );

        Board(const Board& other_board);

        Board& operator=(const Board& other_board);

        // Comparison operators for equality and inequality'
        bool operator==(const Board& other) const;
        bool operator!=(const Board& other) const;

        // Overloaded output stream operator for the board
        friend std::ostream& operator<<(std::ostream& out, const Board& board_class);

        void print_white_perspective(
            std::array<int, 2>& last_move_starting,
            std::array<int, 2>& last_move_ending
        );

        void print_white_perspective(
            std::array<int, 2>& last_move_starting,
            std::array<int, 2>& last_move_ending,
            std::array<int, 2> current_piece,
            Actions possible_actions);

        // Helper functions for managing the board
        std::unique_ptr<Piece> create_piece(
            const char& symbol, // Character representing the piece (e.g., 'K', 'p')
            const int& row, // Row position
            const int& col // Column position
        );

        // Initialize the board with the standard starting position
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board();

        // Initialize the board with a custom configuration
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> create_board(const std::array<std::array<char, 8>, 8>& simplify_board);

        // Flattens all checking positions into an unordered set for faster lookups
        PositionSet flatting_checkin_pieces(
            const PositionMap& checkin_pieces
        ) const;

        // Calculate possible moves for the current player
        void get_possible_actions();

        void get_rating();

        void make_action(int old_row, int old_col, int new_row, int new_col, char symbol);

        Board make_action_board(int old_row, int old_col, int new_row, int new_col, char symbol);

        void check_enpassant(int old_row, int old_col, int new_row);

        void check_castling(int old_row, int old_col);

        std::unique_ptr<Piece> create_promoted_piece_player(int row, int col);

        void computer_action(Game& game);
};

#endif