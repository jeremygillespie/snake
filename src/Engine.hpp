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
    Reversal(Graph *graph) :
      Engine{graph},
      width{graph->width + 1},
      height{graph->height + 1} {
        for (int p = 0; p < graph->size; ++p) {
            if (graph->occupied[p]) {
                for (int i = 0; i < 4; ++i) {
                    bool x_positive = i % 2 == 0;
                    bool y_positive = i / 2 == 0;
                    int cor = corner(p, x_positive, y_positive);
                }
            }
        }
    }

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

    const int width, height;

    int x(int cor) {
        return cor / height;
    }

    int y(int cor) {
        return cor % height;
    }

    bool has_polarity(int cor) {
        for (int i = 0; i < 4; ++i) {
            bool x_positive = i % 2 == 0;
            bool y_positive = i / 2 == 0;
            int p = point(cor, x_positive, y_positive);

            if (p == -1)
                continue;

            if (p == graph->head)
                continue;

            if (graph->occupied[p] == 0)
                continue;

            return true;
        }
        return false;
    }

    bool polarity(int cor) {
        for (int i = 0; i < 4; ++i) {
            bool x_positive = i % 2 == 0;
            bool y_positive = i / 2 == 0;
            int p = point(cor, x_positive, y_positive);

            if (p == -1)
                continue;

            if (p == graph->head)
                continue;

            if (graph->occupied[p] == 0)
                continue;

            Direction incoming = graph->directions[p];
            Direction outgoing = find_outgoing(p);

            return polarity(incoming, outgoing, x_positive, y_positive);
        }
        return false;
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

    bool polarity(
    Direction incoming, Direction outgoing, bool x_positive, bool y_positive) {
        int x = x_positive ? 1 : -1;
        int y = y_positive ? 1 : -1;

        Direction right_inc = incoming + Direction::turn_right;
        Direction right_out = outgoing + Direction::turn_right;

        // determined by incoming
        if (x == (incoming + Direction::turn_reverse).x()) {
            return y == right_inc.y();
        } else if (y == (incoming + Direction::turn_reverse).y()) {
            return x == right_inc.x();
        }
        // determined by outgoing
        else if (x == outgoing.x()) {
            return y == right_out.y();
        } else if (y == outgoing.y()) {
            return x == right_out.x();
        }
        // corner on outside of turn
        else {
            if (outgoing == (incoming + Direction::turn_right)) {
                return false;
            } else {
                return true;
            }
        }
    }

    int corner(int p, bool x_positive, bool y_positive) {
        int result = p;
        if (x_positive) {
            if (x(result) >= width - 1)
                return -1;
            result += height;
        }
        if (y_positive) {
            if (y(result) >= height - 1)
                return -1;
            result += 1;
        }
        return result;
    }

    int point(int cor, bool x_positive, bool y_positive) {
        int result = cor;
        if (!x_positive) {
            if (x(result) == 0)
                return -1;
            result -= height;
        }
        if (!y_positive) {
            if (y(result) == 0)
                return -1;
            result -= 1;
        }
        return result;
    }

    bool safe(Direction dir, int p) {
        // TODO: wall corners

        Direction incoming = graph->directions[p];
        Direction outgoing = dir;

        for (int i = 0; i < 4; ++i) {
            bool x_positive = i % 2 == 0;
            bool y_positive = i / 2 == 0;

            int cor = corner(p, x_positive, y_positive);
            bool pol = polarity(incoming, outgoing, x_positive, y_positive);

            if (has_polarity(cor) && polarity(cor) != pol)
                return false;
        }

        return true;
    }
};

} // namespace snake

#endif