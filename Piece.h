#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>

struct PositionHash {
    std::size_t operator()(const std::array<int, 2>& arr) const {
        return std::hash<int>()(arr[0] * 10 + arr[1]);
    }
};

using PositionSet = std::unordered_set<
    std::array<int, 2>,
    PositionHash
>;

using PositionMap = std::unordered_map<
    std::array<int, 2>,
    PositionSet,
    PositionHash
>;

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

        std::unordered_map<
            char,
            std::unordered_map<
                std::string,
                std::string
            >
        > legend = {
            {'R', {{"piece", "rook"}, {"player", "white"}}},
            {'N', {{"piece", "knight"}, {"player", "white"}}},
            {'B', {{"piece", "bishop"}, {"player", "white"}}},
            {'K', {{"piece", "king"}, {"player", "white"}}},
            {'Q', {{"piece", "queen"}, {"player", "white"}}},
            {'P', {{"piece", "pawn"}, {"player", "white"}}},
            {'r', {{"piece", "rook"}, {"player", "black"}}},
            {'n', {{"piece", "knight"}, {"player", "black"}}},
            {'b', {{"piece", "bishop"}, {"player", "black"}}},
            {'k', {{"piece", "king"}, {"player", "black"}}},
            {'q', {{"piece", "queen"}, {"player", "black"}}},
            {'p', {{"piece", "pawn"}, {"player", "black"}}}
        };

        struct Result {
            std::unordered_set<std::array<int, 2>, PositionHash> moves;
            std::unordered_set<std::array<int, 2>, PositionHash> attacks;
            bool promotion = false;

            bool operator==(const Result& other) const {
            return (this->moves == other.moves &&
                    this->attacks == other.attacks &&
                    this->promotion == other.promotion
                    );
            }

            friend std::ostream& operator<<(std::ostream& out, const Result& res);
        };

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

        Result check_piece_possible_moves (
            Board& board_class,
            PositionSet& attacked_positions,
            PositionMap& checkin_pieces,
            PositionMap& pinned_pieces
        );
};

#endif