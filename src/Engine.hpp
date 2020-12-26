#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include <array>
#include <deque>
#include <random>

#include "Graph.hpp"

namespace snake {

class Engine {
public:
    Engine(
        Graph *graph, bool show_search = false, int width = 0, int height = 0) :
      graph{graph},
      next_move{},
      show_search{show_search},
      search_path{},
      width{width},
      height{height},
      size{width * height},
      corners(size, 0),
      corner_vals(size, 0) {}

    Graph *graph;

    // true if move is updated
    // false if intermediate step
    virtual bool update() = 0;

    virtual void set_move(Direction dir) {}

    virtual void move() {
        graph->move(next_move);
    }

    Direction next_move;

    const bool show_search;
    std::deque<Direction> search_path;

    const int width, height, size;

    int &corner(int p) {
        return corner_vals[corners[p]];
    }

protected:
    std::vector<int> corners;
    std::vector<int> corner_vals;
};

class Human : public Engine {
public:
    Human(Graph *graph) : Engine(graph) {}

    bool update() {
        return true;
    }

    void set_move(Direction dir) {
        next_move = dir;
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

        next_move = best_dir;
        return true;
    }

private:
    static const int cost_unit = 100;
    const int max_cost = cost_unit * (graph->size + 1);

    int cost(Direction dir) {
        if (graph->can_move(dir) == false)
            return max_cost;

        int result =
            cost_unit *
            graph->distance(graph->point(graph->head, dir), graph->apple);

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
    Reversal(Graph *graph, bool show_search) :
      Engine{graph, show_search, graph->width + 1, graph->height + 1},
      occupied(size, false) {

        for (int cor = 0; cor < size; ++cor) {
            if (cor / height == 0 || cor % height == 0 ||
                cor / height == width - 1 || cor % height == height - 1) {
                corners[cor] = 0;
            } else {
                corners[cor] = cor;
            }
        }

        corner_vals[0] = Direction::turn_left;

        for (int p = 0; p < graph->size; ++p) {
            if (graph->occupied[p] != 0) {
                corner(corner_right(p)) =
                    polarity_right(Direction{}, Direction{});
                corner(corner_left(p)) =
                    polarity_left(Direction{}, Direction{});
            }
        }
    }

    bool update() {
        int best_cost = -1;

        for (int i = 0; i < 4; ++i) {
            int c = cost(Direction{i});
            if (c < best_cost || best_cost == -1) {
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

        next_move = best_dir;

        return true;
    }

    void move() {

        Direction incoming = graph->incoming[graph->head];
        Direction outgoing = next_move;
        corner(corner_right(graph->head)) = polarity_right(incoming, outgoing);
        corner(corner_left(graph->head)) = polarity_left(incoming, outgoing);

        graph->move(next_move);

        std::fill(occupied.begin(), occupied.end(), false);

        for (int cor = 0; cor < size; ++cor) {
            for (int d = 0; d < 4; ++d) {
                int p = point(cor, Direction{d});
                if (p != -1 && graph->occupied[p] > 1 && p != graph->head) {
                    occupied[corners[cor]] = true;
                }
            }
        }

        for (int cor = 0; cor < size; ++cor) {
            if (!occupied[corners[cor]])
                corner(cor) = Direction::turn_none;
        }
    }

private:
    std::vector<bool> occupied;

    int cost(Direction dir) {
        int result =
            10 * graph->distance(graph->point(graph->head, dir), graph->apple);

        if (graph->can_move(dir) == false)
            return result + 100;

        if (safe(dir, graph->head) == false)
            return result + 50;

        if (dir != graph->incoming[graph->head])
            return result + 1;

        return result;
    }

    int polarity_right(Direction incoming, Direction outgoing) {
        if (outgoing == incoming + Direction::turn_right)
            return Direction::turn_left;
        else
            return Direction::turn_right;
    }

    int polarity_left(Direction incoming, Direction outgoing) {
        if (outgoing == incoming + Direction::turn_left)
            return Direction::turn_right;
        else
            return Direction::turn_left;
    }

    int corner_right(int p) {
        Direction incoming = graph->incoming[p];

        int dx = incoming.x() + (incoming + Direction::turn_right).x();
        int dy = incoming.y() + (incoming + Direction::turn_right).y();

        int x = graph->x(p);
        if (dx > 0)
            ++x;

        int y = graph->y(p);
        if (dy > 0)
            ++y;

        return x * height + y;
    }

    int corner_left(int p) {
        Direction incoming = graph->incoming[p];

        int dx = incoming.x() + (incoming + Direction::turn_left).x();
        int dy = incoming.y() + (incoming + Direction::turn_left).y();

        int x = graph->x(p);
        if (dx > 0)
            ++x;

        int y = graph->y(p);
        if (dy > 0)
            ++y;

        return x * height + y;
    }

    int point(int cor, Direction dir) {
        int dx = dir.x() + (dir + Direction::turn_right).x();
        int dy = dir.y() + (dir + Direction::turn_right).y();

        int x = cor / height;
        if (dx < 0)
            --x;

        int y = cor % height;
        if (dy < 0)
            --y;

        return graph->point(x, y);
    }

    bool safe(Direction dir, int p) {
        Direction incoming = graph->incoming[p];
        Direction outgoing = dir;

        int cor = corner_left(p);
        if (corner(cor) != Direction::turn_none &&
            corner(cor) != polarity_left(incoming, outgoing))
            return false;

        cor = corner_right(p);
        if (corner(cor) != Direction::turn_none &&
            corner(cor) != polarity_right(incoming, outgoing))
            return false;

        return true;
    }
};

} // namespace snake

#endif