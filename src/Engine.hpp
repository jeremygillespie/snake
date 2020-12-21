#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include <array>
#include <random>
#include <stack>

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(Graph *graph) : graph{graph}, move{}, path{} {}

    Graph *graph;

    virtual void initialize() {}

    virtual bool update() = 0;

    virtual void set_move(Direction dir) {}

    Direction move;

    std::stack<Direction> path;

    int search_pos;
};

class Human : public Engine {
public:
    Human(Graph *graph) : Engine{graph} {}

    bool update() {
        search_pos = graph->head;
        return true;
    }

    void set_move(Direction dir) {
        move = dir;
    }
};

class Manhattan : public Engine {
public:
    Manhattan(Graph *graph) : Engine{graph} {}

    bool update() {
        int best_cost = max_cost;

        for (int i = 0; i < 4; ++i) {
            int c = cost(Direction{i});
            if (c < best_cost) {
                best_cost = c;
            }
        }

        Direction best_dir;
        int c = -1;
        do {
            best_dir = {
            std::uniform_int_distribution<>(0, 3)(*graph->r_engine)};
            c = cost(best_dir);
        } while (c != best_cost);

        move = best_dir;
        search_pos = graph->head;
        return true;
    }

private:
    static const int cost_unit = 100;
    const int max_cost = cost_unit * (graph->size + 1);

    int cost(Direction dir) {
        if (graph->can_move(dir) == false)
            return max_cost;

        int result = cost_unit * graph->distance(
                                 graph->point(graph->head, dir), graph->apple);

        if (dir != graph->directions[graph->head])
            result += 1;

        if (!safe_move(dir, graph->head))
            result += cost_unit * 4;

        return result;
    }

    bool safe_move(Direction dir, int point) {
        switch (dir.value()) {
        // positive y
        case Direction::north:
            // even column
            return graph->x(point) % 2 == 0;
        // negative y
        case Direction::south:
            // odd column
            return graph->x(point) % 2 == 1;
        // negative x
        case Direction::west:
            // even row
            return graph->y(point) % 2 == 0;
        // positive x
        default:
            // odd row
            return graph->y(point) % 2 == 1;
        }
    }
};

} // namespace snake

#endif