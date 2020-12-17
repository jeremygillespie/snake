#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include <array>

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(const Graph &graph) : graph{graph} {}

    Graph graph;

    virtual void initialize() {}

    virtual bool update(Direction dir = {}) {
        if (dir == graph.directions[graph.head] + Direction::turn_reverse)
            dir += Direction::turn_reverse;
        if (graph.can_move(dir) == false)
            return false;
        graph.move(dir);
        return true;
    }
};

class Naive : public Engine {
public:
    Naive(const Graph &graph) : Engine{graph} {}

    bool update(Direction) {
        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            if (graph.can_move(dir) && good_move(dir)) {
                graph.move(dir);
                return true;
            }
        }
        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            if (graph.can_move(dir)) {
                graph.move(dir);
                return true;
            }
        }
        return false;
    }

private:
    bool good_move(Direction dir) {
        return graph.distance(graph.point(graph.head, dir), graph.apple) <
               graph.distance(graph.head, graph.apple);
    }
};

class Manhattan : public Engine {
public:
    Manhattan(const Graph &graph) : Engine{graph} {}

    bool update(Direction) {
        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            if (graph.can_move(dir) && safe_move(dir) && good_move(dir)) {
                graph.move(dir);
                return true;
            }
        }
        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            if (graph.can_move(dir) && safe_move(dir)) {
                graph.move(dir);
                return true;
            }
        }
        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            if (graph.can_move(dir)) {
                graph.move(dir);
                return true;
            }
        }
        return false;
    }

private:
    bool good_move(Direction dir) {
        return graph.distance(graph.point(graph.head, dir), graph.apple) <
               graph.distance(graph.head, graph.apple);
    }

    bool safe_move(Direction dir) {}
};

} // namespace snake

#endif