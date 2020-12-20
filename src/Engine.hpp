#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include <array>
#include <iostream>
#include <random>

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

protected:
    static const int cost_unit = 100;
    const int max_cost = cost_unit * (graph.size + 1);
};

class Naive : public Engine {
public:
    Naive(const Graph &graph) : Engine{graph} {}

    bool update(Direction) {
        int best_cost = max_cost;

        for (int i = 0; i < 4; ++i) {
            int c = cost(Direction{i});
            if (c < best_cost) {
                best_cost = c;
            }
        }

        if (best_cost == max_cost)
            return false;

        Direction best_dir;
        int c = -1;
        do {
            best_dir = {std::uniform_int_distribution<>(0, 3)(*graph.r_engine)};
            c = cost(best_dir);
        } while (c != best_cost);

        graph.move(best_dir);
        return true;
    }

private:
    int cost(Direction dir) {
        if (graph.can_move(dir) == false)
            return max_cost;

        int result = cost_unit *
                     graph.distance(graph.point(graph.head, dir), graph.apple);

        if (dir != graph.directions[graph.head])
            result += 1;

        return result;
    }
};

class Manhattan : public Engine {
public:
    Manhattan(const Graph &graph) : Engine{graph} {}

    bool update(Direction) {
        int best_cost = max_cost;

        for (int i = 0; i < 4; ++i) {
            int c = cost(Direction{i});
            if (c < best_cost) {
                best_cost = c;
            }
        }

        if (best_cost == max_cost)
            return false;

        Direction best_dir;
        int c = -1;
        do {
            best_dir = {std::uniform_int_distribution<>(0, 3)(*graph.r_engine)};
            c = cost(best_dir);
        } while (c != best_cost);

        graph.move(best_dir);
        return true;
    }

private:
    int cost(Direction dir) {
        if (graph.can_move(dir) == false)
            return max_cost;

        int result = cost_unit *
                     graph.distance(graph.point(graph.head, dir), graph.apple);

        if (dir != graph.directions[graph.head])
            result += 1;

        if (!safe_move(dir, graph.head))
            result += cost_unit * 4;

        return result;
    }

    bool safe_move(Direction dir, int point) {
        switch (dir.value()) {
        // positive y
        case Direction::north:
            // even column
            return graph.x(point) % 2 == 0;
        // negative y
        case Direction::south:
            // odd column
            return graph.x(point) % 2 == 1;
        // negative x
        case Direction::west:
            // even row
            return graph.y(point) % 2 == 0;
        // positive x
        default:
            // odd row
            return graph.y(point) % 2 == 1;
        }
    }
};

} // namespace snake

#endif