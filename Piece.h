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

        Piece(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Piece& piece);

        bool is_valid_position(const Board& board, const int& row, const int& column) const;

        bool is_not_pinned(
            const std::array<int, 2>& piece_position,
            const std::array<int, 2>& move,
            const Board& board_class,
            const PositionMap& pinned_pieces
        ) const;

        PositionSet flatting_checkin_pieces(
            const PositionMap& checkin_pieces
        ) const;

        void rook_bishop_queen_template (
            Board& board_class,
            const std::vector<std::array<int, 2>>& directions,
            const bool& opponent,
            const PositionSet& checking_positions
        );

        virtual void check_piece_possible_moves (
            Board& board_class
        ) = 0;
};

class Pawn: public Piece {
    public:
        Pawn(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

class Knight: public Piece {
    public:
        Knight(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

class King: public Piece {
    public:
        King(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

class Rook: public Piece {
    public:
        Rook(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

class Bishop: public Piece {
    public:
        Bishop(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

class Queen: public Piece {
    public:
        Queen(const char& input_symbol,
            const std::string& input_piece,
            const std::string& input_player,
            const int& input_row,
            const int& input_column
        );

        void check_piece_possible_moves (
            Board& board_class
        ) override;
};

#endif