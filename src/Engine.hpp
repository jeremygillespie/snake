#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include <array>
#include <deque>
#include <random>

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(Graph *graph, bool show_search = false) :
      graph{graph},
      move{},
      search_path{},
      show_search{show_search} {}

    Graph *graph;

    // true if move is updated
    // false if intermediate step
    virtual bool update() = 0;

    virtual void set_move(Direction dir) {}

    Direction move;

    std::deque<Direction> search_path;

    const bool show_search;
};

class Human : public Engine {
public:
    Human(Graph *graph) : Engine(graph) {}

    bool update() {
        return true;
    }

    void set_move(Direction dir) {
        move = dir;
    }
};

class Manhattan : public Engine {
public:
    Manhattan(Graph *graph) : Engine(graph) {}

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

        if (!safe(dir, graph->head))
            result += cost_unit * 4;

        return result;
    }

    bool safe(Direction dir, int point) {
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

class Reversal : public Engine {
public:
    Reversal(Graph *graph) : Engine{graph} {}

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
        return true;
    }

private:
    static constexpr int cost_unsafe = 100;
    static constexpr int cost_distance = 0;
    static constexpr int cost_turn = 0;

    const int max_cost = cost_unsafe * 2;

    int cost(Direction dir) {
        if (graph->can_move(dir) == false)
            return max_cost;

        int result = cost_distance *
                     graph->distance(
                     graph->point(graph->head, dir), graph->apple);

        if (dir != graph->directions[graph->head])
            result += cost_turn;

        if (!safe(dir, graph->head))
            result += cost_unsafe;

        return result;
    }

    Direction find_outgoing(int p) {
        for (int d = 0; d < 4; ++d) {
            int p2 = graph->point(p, {d});

            if (p2 == -1)
                continue;

            Direction dir = graph->directions[p2];

            if (graph->point(p, dir + Direction::turn_reverse) == p)
                return dir;
        }

        return {};
    }

    bool safe(Direction dir, int p) {
        // TODO: wall corners

        Direction incoming = graph->directions[p];
        Direction outgoing = dir;

        int dx_r = incoming.x() + (incoming + Direction::turn_right).x();
        int dy_r = incoming.y() + (incoming + Direction::turn_right).y();

        int dx_l = incoming.x() + (incoming + Direction::turn_left).x();
        int dy_l = incoming.y() + (incoming + Direction::turn_left).y();

        return true;
    }
};

} // namespace snake

#endif