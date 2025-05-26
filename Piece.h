#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include <utility>

#include "Types.h"

// Forward declaration of the Board class
class Board;

// Base class representing a generic chess piece
class Piece {
    public:
        char symbol; // Character symbol representing the piece (e.g., 'P' for pawn)
        PieceType piece; // Name of the piece (e.g., "pawn")
        PlayerColor player; // Player owning the piece ("white" or "black")
        int row; // Row position of the piece on the board
        int column; // Column position of the piece on the board

        Actions possible_actions; // Stores the possible actions for the piece

        // Constructor
        Piece(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        virtual ~Piece() = default;

        // Comparison operators for equality and inequality
        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;

        // Overloaded output stream operator for Piece
        friend std::ostream& operator<<(std::ostream& out, const Piece& piece);

        // Check if a position is within the board bounds
        bool is_valid_position(const Board& board, const int& row, const int& column) const;

        // Checks if moving the piece will not expose the king to a check (i.e., the piece is not pinned)
        bool is_not_pinned(
            const std::array<int, 2>& piece_position,
            const std::array<int, 2>& move,
            const Board& board_class,
            const PositionMap& pinned_pieces
        ) const;

        // Helper method for rook, bishop, and queen movement logic
        void rook_bishop_queen_move_template_active_player (
            Board& board_class,
            const std::vector<std::array<int, 2>>& directions
        );

        void rook_bishop_queen_move_template_opponent (
            Board& board_class,
            const std::vector<std::array<int, 2>>& directions,
            PositionSet checking_positions
        );

        void rook_bishop_queen_rating_template_active_player (
            Board& board_class,
            const std::vector<std::array<int, 2>>& directions
        );

        void rook_bishop_queen_rating_template_opponent (
            Board& board_class,
            const std::vector<std::array<int, 2>>& directions,
            PositionSet checking_positions
        );

        // Pure virtual function to determine piece-specific possible moves
        virtual void check_piece_possible_moves_opponent (
            Board& board_class
        ) = 0;

        virtual void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) = 0;

        virtual int const get_value() const = 0;
        
        bool check_if_legal_action(int check_row, int check_col);

        virtual void update_rating_opponent (
            Board& board_class
        ) = 0;

        virtual void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) = 0;

        void update_move_rating_helping(
            Board& board_class,
            PlayerColor player,
            int row,
            int col
        );
};

// Derived class representing a Pawn
class Pawn: public Piece {
    public:
        Pawn(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
         ) override;

        int const get_value() const override {return 1;};
};

// Derived class representing a Knight
class Knight: public Piece {
    public:
        Knight(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        int const get_value() const override {return 3;};
};

// Derived class representing a King
class King: public Piece {
    public:
        King(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        int const get_value() const override {return 50;};
};

// Derived class representing a Rook
class Rook: public Piece {
    public:
        Rook(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        int const get_value() const override {return 5;};
};

// Derived class representing a Bishop
class Bishop: public Piece {
    public:
        Bishop(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        int const get_value() const override {return 3;};
};

// Derived class representing a Queen
class Queen: public Piece {
    public:
        Queen(const char& input_symbol,
            const PieceType& input_piece,
            const PlayerColor& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves_opponent (
            Board& board_class
        ) override;

        void check_piece_possible_moves_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        void update_rating_opponent (
            Board& board_class
        ) override;

        void update_rating_active_player (
            Board& board_class,
            PositionSet checking_positions
        ) override;

        int const get_value() const override {return 9;};
};

#endif