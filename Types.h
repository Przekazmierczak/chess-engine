#ifndef TYPES_H
#define TYPES_H

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
std::ostream& operator<<(std::ostream& out, const PositionSet& set);

std::array<int, 2> get_random_element(PositionSet set);

using PositionMap = std::unordered_map<
    std::array<int, 2>,
    PositionSet,
    PositionHash
>;
std::ostream& operator<<(std::ostream& out, const PositionMap& set);

struct Actions {
    PositionSet moves; // Set of valid move positions
    PositionSet attacks; // Set of valid attack positions
    bool promotion = false; // Indicates if a pawn promote in next move

    // Equality operator for comparing Actions
    bool operator==(const Actions& other) const;

    void reset();

    // Overloaded output stream operator for Actions
    friend std::ostream& operator<<(std::ostream& out, const Actions& res);
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