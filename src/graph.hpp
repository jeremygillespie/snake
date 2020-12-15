#ifndef SNAKE_GRAPH_HPP
#define SNAKE_GRAPH_HPP

#include <random>
#include <vector>

namespace snake {

struct Direction {
public:
    static constexpr int north = 0, east = 1, south = 2, west = 3,
                         turn_left = -1, turn_none = 0, turn_right = 1,
                         turn_reverse = 2;

    const int value;

    Direction(int value = north) :
      value{value < 0 ? (value % 4 + 4) % 4 : value % 4} {}

    int x() const {
        return value % 2 == 0 ? 0 : (value == 1 ? 1 : -1);
    }
    int y() const {
        return value % 2 == 0 ? (value == 0 ? 1 : -1) : 0;
    }

    bool operator==(const Direction &other) const {
        return value == other.value;
    }

    bool operator!=(const Direction &other) const {
        return value != other.value;
    }

    Direction operator+(int turn) const {
        return Direction(value + turn);
    }
};

class Graph {
public:
    const int width, height, size;
    int head, apple, length;

    std::vector<int> occupied;
    std::vector<bool> walls;
    std::vector<Direction> Directions;

    Graph(int width, int height, int x, int y, int length) :
      width{width},
      height{height},
      size{width * height},
      length{length},
      occupied(size, false),
      walls(size, false),
      Directions(size, {}) {}
};

} // namespace snake

#endif