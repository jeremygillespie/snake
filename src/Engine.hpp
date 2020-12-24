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

        if (dir != graph->incoming[graph->head])
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
        int best_cost = 1;

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

        if (edge_polarity_dur > 0)
            --edge_polarity_dur;

        return true;
    }

private:
    int cost(Direction dir) {
        if (graph->can_move(dir) == false)
            return 1;

        if (!safe(dir, graph->head))
            return 1;

        return 0;
    }

    int edge_polarity_dur = 0;
    bool edge_polarity = false;

    bool safe(Direction dir, int p) {
        bool polarity;
        bool edge = false;
        if (graph->x(p) == 0) {
            edge = true;
            polarity = dir.value() == Direction::north;
        } else if (graph->y(p) == 0) {
            edge = true;
            polarity = dir.value() == Direction::west;
        } else if (graph->x(p) == graph->width - 1) {
            edge = true;
            polarity = dir.value() == Direction::south;
        } else if (graph->y(p) == graph->height - 1) {
            edge = true;
            polarity = dir.value() == Direction::east;
        }

        if (edge) {
            if (edge_polarity_dur > 0) {
                if (edge_polarity == polarity) {
                    edge_polarity_dur = graph->length;
                    return true;
                } else {
                    return false;
                }
            } else {
                edge_polarity_dur = graph->length;
                edge_polarity = polarity;
                return true;
            }
        }

        Direction in = graph->incoming[p];
        Direction out = dir;

        int p_new = graph->point(p, out);
        int p_old = graph->point(p, in + Direction::turn_reverse);

        for (int i = 0; i < 4; ++i) {
            Direction dir_offset{i};
            int p_comp = p;
            p_comp = graph->point(p_comp, dir_offset);
            p_comp = graph->point(p_comp, dir_offset + Direction::turn_right);

            if (graph->occupied[p_comp] == 0)
                continue;
            if (graph->incoming[p_comp] == out && graph->outgoing[p_comp] == in)
                return false;
        }

        for (int i = 0; i < 4; ++i) {
            Direction dir_offset{i};
            if (
            dir_offset == out || dir_offset == out + Direction::turn_reverse)
                continue;

            int p_comp = p_new;
            p_comp = graph->point(p_comp, dir_offset);

            if (graph->occupied[p_comp] <= 1)
                continue;
            if (graph->outgoing[p_comp] == out)
                return false;
            if (graph->incoming[p_comp] == out)
                return false;
        }

        for (int i = 0; i < 4; ++i) {
            Direction dir_offset{i};
            if (
            dir_offset == in + Direction::turn_reverse || dir_offset == out ||
            dir_offset == out + Direction::turn_reverse)
                continue;

            int p_comp = p;
            p_comp = graph->point(p_comp, dir_offset);

            if (graph->occupied[p_comp] <= 1)
                continue;
            if (graph->outgoing[p_comp] == out)
                return false;
            if (graph->incoming[p_comp] == out)
                return false;
        }

        return true;
    }
};

} // namespace snake

#endif