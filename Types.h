#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include <compare>

struct PositionHash {
    std::size_t operator()(const std::array<int, 2>& arr) const {
        return std::hash<int>()(arr[0] * 10 + arr[1]);
    }
};

using PositionSet = std::unordered_set<
    std::array<int, 2>,
    PositionHash
>;
std::ostream& operator<<(std::ostream& out, const PositionSet& set);

using PositionMap = std::unordered_map<
    std::array<int, 2>,
    PositionSet,
    PositionHash
>;
std::ostream& operator<<(std::ostream& out, const PositionMap& set);

enum PlayerColor {
    white,
    black
};

enum PieceType {
    pawn,
    rook,
    knight,
    bishop,
    queen,
    king
};

enum Winner {
    whiteWin,
    blackWin,
    draw,
    notFinished
};

struct Actions {
    PositionSet moves; // Set of valid move positions
    PositionSet attacks; // Set of valid attack positions
    bool promotion = false; // Indicates if a pawn promote in next move

    // Equality operator for comparing Actions
    bool operator==(const Actions& other) const;

    void reset();

    // Overloaded output stream operator for Actions
    friend std::ostream& operator<<(std::ostream& out, const Actions& res);

    class Iterator{
    private:
        const Actions& container;
        PositionSet::iterator current;
        bool in_attacks;

    public:
        Iterator(
            const Actions& c,
            const PositionSet::iterator& it,
            const bool& attacks_flag
        );

        const std::array<int, 2>& operator*() const;
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    Iterator begin() const {
        if (!attacks.empty()) {
            return Iterator(*this, attacks.begin(), true);
        } else {
            return Iterator(*this, moves.begin(), false);
        }
    }

    Iterator end() const {
        return Iterator(*this, moves.end(), false);
    }
};

struct Action {
    std::array<int, 2> old_position;
    std::array<int, 2> new_position;
    char symbol;
    int rating;

    Action(const std::array<int, 2>& input_old_position, const std::array<int, 2>& input_new_position, const char& input_symbol, const int& input_rating);

    std::strong_ordering operator<=>(const Action& other) const;
    bool operator==(const Action& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Action& action);
};

struct Notation {
    int row;
    char column;

    Notation(const std::string& input_position);
    Notation(const int& row, const int& col);

    std::array<int, 2> parse_square_notation() const;

    friend std::ostream& operator<<(std::ostream& out, const Notation& notation);
};


#endif