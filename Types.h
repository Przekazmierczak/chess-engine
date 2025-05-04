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


#endif