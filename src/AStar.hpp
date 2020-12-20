#ifndef SNAKE_ASTAR_HPP
#define SNAKE_ASTAR_HPP

#include <queue>

#include "Engine.hpp"

namespace snake {

class AStar : public Engine {
public:
    AStar(Graph *graph) : Engine{graph}, path{}, manhattan{graph} {}

    Direction next_move() {
        if (path.empty()) {
            search();

            if (path.empty())
                return no_path();
        }
        Direction result = path.front();
        path.pop_front();
        return result;
    }

private:
    void search() {}

    Manhattan manhattan;

    Direction no_path() {
        return manhattan.next_move();
    }

    std::deque<Direction> path;
};

} // namespace snake

#endif