#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include <compare>

// Custom hash function for positions represented as a 2D array
struct PositionHash {
    std::size_t operator()(const std::array<int, 2>& arr) const {
        return std::hash<int>()(arr[0] * 10 + arr[1]);
    }
};

// Set of positions with custom hashing
using PositionSet = std::unordered_set<
    std::array<int, 2>,
    PositionHash
>;
std::ostream& operator<<(std::ostream& out, const PositionSet& set);

// Map of positions to sets of positions with custom hashing
using PositionMap = std::unordered_map<
    std::array<int, 2>,
    PositionSet,
    PositionHash
>;
std::ostream& operator<<(std::ostream& out, const PositionMap& set);

// Enum representing player colors
enum PlayerColor {
    white,
    black
};

// Enum representing chess piece types
enum PieceType {
    pawn,
    rook,
    knight,
    bishop,
    queen,
    king
};

// Enum representing the game's outcome
enum Winner {
    whiteWin,
    blackWin,
    draw,
    notFinished
};

// Represents possible actions for a piece
struct Actions {
    PositionSet moves; // Set of valid move positions
    PositionSet attacks; // Set of valid attack positions
    bool promotion = false; // Indicates if a pawn promote in next move

    // Equality operator for comparing Actions
    bool operator==(const Actions& other) const;

     // Reset all actions
    void reset();

    // Overloaded output stream operator for Actions
    friend std::ostream& operator<<(std::ostream& out, const Actions& res);

    // Iterator for traversing attacks and moves
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

// Represents a single chess action
struct Action {
    std::array<int, 2> old_position; // Starting position
    std::array<int, 2> new_position; // Ending position
    char symbol; // Symbol of the piece
    int rating; // Rating of the action

    Action(
        const std::array<int, 2>& input_old_position,
        const std::array<int, 2>& input_new_position,
        const char& input_symbol, const int& input_rating
    );

    std::strong_ordering operator<=>(const Action& other) const;
    bool operator==(const Action& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Action& action);
};

// Represents chess notation for positions
struct Notation {
    int row;
    char column;

    Notation(const std::string& input_position); // Initialize from notation string
    Notation(const int& row, const int& col); // Initialize from indices

    std::array<int, 2> parse_square_notation() const; // Convert notation to indices

    friend std::ostream& operator<<(std::ostream& out, const Notation& notation);
};


#endif