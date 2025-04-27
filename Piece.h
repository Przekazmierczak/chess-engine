#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include <utility>

#include "Types.h"

class Board;

class Piece {
    public:
        char symbol;
        std::string piece;
        std::string player;
        int row;
        int column;

        struct Actions {
            PositionSet moves;
            PositionSet attacks;
            bool promotion = false;

            bool operator==(const Actions& other) const {
            return (this->moves == other.moves &&
                    this->attacks == other.attacks &&
                    this->promotion == other.promotion
                    );
            }

            friend std::ostream& operator<<(std::ostream& out, const Actions& res);
        };

        Actions possible_actions;

        Piece(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column);

        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Piece& piece);

        bool is_valid_position(Board& board, int row, int column);

        bool is_not_pinned(
            std::array<int, 2> piece_position,
            std::array<int, 2> move,
            Board& board_class,
            PositionMap& pinned_pieces
        );

        PositionSet flatting_checkin_pieces(
            PositionMap& checkin_pieces
        );

        virtual void check_piece_possible_moves (
            Board& board_class
        );

        void rook_bishop_queen_template (
            Board& board_class,
            std::vector<std::array<int, 2>> directions,
            bool opponent,
            PositionSet checking_positions
        );
};

class Pawn: public Piece {
    public:
        Pawn(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

class Knight: public Piece {
    public:
        Knight(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

class King: public Piece {
    public:
        King(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

class Rook: public Piece {
    public:
        Rook(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

class Bishop: public Piece {
    public:
        Bishop(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

class Queen: public Piece {
    public:
        Queen(char input_symbol, std::string input_piece, std::string input_player, int input_row, int input_column): Piece(input_symbol, input_piece, input_player, input_row, input_column) {}
        void check_piece_possible_moves (
            Board& board_class
        );
};

#endif