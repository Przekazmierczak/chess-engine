#include "Board.h"
#include "Piece.h"
#include "Types.h"

std::ostream& operator<<(std::ostream& out, const PositionSet& set) {
    bool first = true;
    out << "{";
    for (const auto& position : set) {
        if (first) {
            first = false;
        } else {
            out << ", ";}

        out << "{" << position[0] << ", " << position[1] << "}";
    }
    out << "}";
    return out;
};

std::ostream& operator<<(std::ostream& out, const PositionMap& map) {
    bool firstOuter  = true;

    out << "[";
    for (const auto& position : map) {
        if (firstOuter) {
            firstOuter = false;
        } else {
            out << ", ";}

        out << "{" << position.first[0] << ", " << position.first[1] << "}: ";

        std::cout << position.second;
    }
    out << "]";
    return out;
};

bool Actions::operator==(const Actions& other) const {
    return (this->moves == other.moves &&
            this->attacks == other.attacks &&
            this->promotion == other.promotion
            );
}

void Actions::reset() {
    moves = {};
    attacks = {};
    promotion = false;
}

// Overloaded output stream operator for Actions
std::ostream& operator<<(std::ostream& out, const Actions& res) {
    bool first = true;
    out << "Moves: {";
    for (auto move : res.moves) {
        if (first) {
            first = false;
        } else {
            out << ", ";}
        out << "{" << move[0] << ", " << move[1] << "}";
    }
    out << "}, ";

    first = true;
    out << "Attacks: {";
    for (auto attack : res.attacks) {
        if (first) {
            first = false;
        } else {
            out << ", ";}
        out << "{" << attack[0] << ", " << attack[1] << "}, ";
    }
    out << "}, ";

    out << "Promotion: " << (res.promotion ? "true":"false");

    return out;
};


Actions::Iterator::Iterator(
    const Actions& c,
    PositionSet::iterator it,
    bool attacks_flag)
    : container(c),
      current(it),
      in_attacks(attacks_flag) {
}

const std::array<int, 2>& Actions::Iterator::operator*() const {
    return *current;
}

Actions::Iterator& Actions::Iterator::operator++() {
    ++current;
    if (in_attacks && current == container.attacks.end()) {
        current = container.moves.begin();
        in_attacks = false;
    }
    return *this;
}

bool Actions::Iterator::operator==(const Iterator& other) const {
    return current == other.current && in_attacks == other.in_attacks;
}

bool Actions::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

Action::Action(std::array<int, 2> input_old_position, std::array<int, 2> input_new_position, char input_symbol, int input_rating) {
    old_position = input_old_position;
    new_position = input_new_position;
    symbol = input_symbol;
    rating = input_rating;
}

std::strong_ordering Action::operator<=>(const Action& other) const {
    return rating <=> other.rating;
}

std::ostream& operator<<(std::ostream& out, const Action& action) {
    out << "[" << action.rating << ": {("
               << action.old_position[0] << "," << action.old_position[1] << "), ("
               << action.new_position[0] << "," << action.new_position[1] << ") "
               << action.symbol << "}], ";
    return out;
}

Notation::Notation(std:: string position)
    : row(int(position[0]) - 48),
      column(position[1]) {
}

Notation::Notation(int row, int col)
    : row(row + 1),
      column(char(104 - col)) {
}

std::array<int, 2> Notation::parse_square_notation() {
    std::array<int, 2> position;

    position[0] = row - 1;
    position[1] = abs(int(column) - 104);

    return position;
}

std::ostream& operator<<(std::ostream& out, const Notation& notation) {
    out << notation.row << notation.column;
    return out;
};