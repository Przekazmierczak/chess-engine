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
        const int ROWS = 8;
        const int COLS = 8;
        char symbol;
        int row;
        int column;
        std::string piece;
        std::string player;

        struct Actions {
            std::unordered_set<std::array<int, 2>, PositionHash> moves;
            std::unordered_set<std::array<int, 2>, PositionHash> attacks;
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

        Piece(char input_symbol, int input_row, int input_column);

        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Piece& piece);

        bool is_valid_position(int row, int column);

        bool is_not_pinned(
            std::array<int, 2> piece_position,
            std::array<int, 2> move,
            Board& board_class,
            PositionMap& pinned_pieces
        );

        PositionSet flatting_checkin_pieces(
            PositionMap& checkin_pieces
        );

        void check_piece_possible_moves (
            Board& board_class
        );
};

#endif