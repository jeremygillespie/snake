#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include "AppleSearch.hpp"
#include "SafetySearch.hpp"

namespace Snake {

struct Cost {
    float death;
    float moves;

    Cost operator+(const Cost &other) const {
        return Cost{moves + other.moves, death + other.death};
    }

    void operator+=(const Cost &other) {
        moves += other.moves;
        death += other.death;
    }

    Cost operator/(int n) const { return Cost{death / n, moves / n}; }

    bool operator==(const Cost &other) const {
        return death == other.death && moves == other.moves;
    }

    bool operator<(const Cost &other) const {
        return death == other.death ? moves < other.moves : death < other.death;
    }

    bool operator>(const Cost &other) const {
        return death == other.death ? moves > other.moves : death > other.death;
    }

    bool operator<=(const Cost &other) const {
        return death < other.death ||
               (death == other.death && moves <= other.moves);
    }

    bool operator>=(const Cost &other) const {
        return death > other.death ||
               (death == other.death && moves >= other.moves);
    }
};

class Engine {
public:
    virtual Path getPath(int nextApple) = 0;
};

class Exhaustive : public Engine {
public:
    Exhaustive(const State &start) : start{start} {}

    Path getPath(int nextApple) {
        DepthFirst search(start);
        Path path{};

        search = DepthFirst(start);

        Path best;
        Cost bestCost{1.0f, 0.0f};
        while (search(path)) {
            Cost c = evalApple(path.end);
            c.moves += path.moves.size();
            if (c < bestCost) {
                best = path;
                bestCost = c;
            }
        }

        std::cout << "\n";

        start = best.end;
        for (int i = 0; i < nextApple; ++i) {
            start = State::nextApple(start);
        }

        return best;
    }

private:
    State start;

    // least cost among possible paths
    Cost evalPath(const State &state) {
        DepthFirst search(state);
        Path path;

        // death if no paths
        Cost best = Cost{1.0f, 0.0f};
        while (search(path)) {
            Cost c = evalApple(path.end);
            c.moves += path.moves.size();
            if (c < best) {
                best = c;
            }
        }

        return best;
    }

    // average cost among possible apples
    Cost evalApple(const State &state) {

        // success if no more apples
        Cost sum{0.0f, 0.0f};
        if (state.length == State::SIZE) {
            return sum;
        }

        int n = State::SIZE - state.length;
        State s = state;

        for (int i = 0; i < n; ++i) {
            s = State::nextApple(s);
            sum += evalPath(s);
        }

        return sum / n;
    }
};

} // namespace Snake

#endif