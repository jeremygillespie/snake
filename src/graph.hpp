#ifndef SNAKE_GRAPH_HPP
#define SNAKE_GRAPH_HPP

#include <vector>

namespace snake {

class graph {
public:
    int size_x, size_y, length;

    std::vector<bool> occupied;
    std::vector<bool> wall;

    graph() : size_x{8}, size_y{8}, length{4}, occupied{}, wall{} {
    }

    void init(int x, int y) {
        size_x = x;
        size_y = y;
        occupied.resize(size(), false);
        wall.resize(size(), false);
    }

    int size() {
        return size_x * size_y;
    }
};

} // namespace snake

#endif