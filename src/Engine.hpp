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
    Manhattan(const Graph &graph) : Engine{graph}, r_engine{} {}

    bool update(Direction) {
        int best_reward = -1;
        int num_best = 0;

        for (int i = 0; i < 4; ++i) {
            Direction dir{i};
            int rew = reward(dir);
            if (rew == best_reward) {
                ++num_best;
            } else if (rew > best_reward) {
                best_reward = rew;
                num_best = 1;
            }
        }

        if (best_reward == -1)
            return false;

        Direction best_dir;
        int rew = -1;
        do {
            best_dir = {std::uniform_int_distribution<>(0, 3)(r_engine)};
            rew = reward(best_dir);
        } while (rew < best_reward);

        graph.move(best_dir);
        return true;
    }

private:
    int reward(Direction dir) {
        if (graph.can_move(dir) == false)
            return -1;

        int value = 1 + graph.distance(graph.head, graph.apple) -
                    graph.distance(graph.point(graph.head, dir), graph.apple);

        value *= 10;

        if (dir == graph.directions[graph.head])
            ++value;

        if (safe_move(dir, graph.head))
            value += 40;

        return value;
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

    std::default_random_engine r_engine;
};

} // namespace snake

#endif