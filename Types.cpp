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

std::array<int, 2> get_random_element(PositionSet set) {
    std::srand(std::time(nullptr));
    int rand_index = rand() % set.size();

    auto iterator = set.begin();
    std::advance(iterator, rand_index);

    return *iterator;
}

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
