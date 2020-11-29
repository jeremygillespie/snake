#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "AppleSearch.hpp"
#include "SafetySearch.hpp"

namespace Snake {

struct Cost {
    float moves;
    float death;

    Cost operator+(const Cost &other) const {
        return Cost{moves + other.moves, death + other.death};
    }

    Cost operator+=(const Cost &other) {
        moves += other.moves;
        death += other.death;
    }

    Cost operator/(int n) const { return Cost{moves / n, death / n}; }

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
    virtual AppleSearch::Path getPath(int nextApple) = 0;
};

class Exhaustive : public Engine {
public:
    Exhaustive(const State &start) : start{start} {}

    AppleSearch::Path getPath(int nextApple) {

        AppleSearch::Path path;
        // generate all possible paths from start
        // return the one with the least cost
        start = path.end;

        return path;
    }

private:
    State start;

    // least cost among possible paths
    Cost evalPath(const State &state) {
        AppleSearch::DepthFirst search(state);
        AppleSearch::Path path;

        Cost best;
        while (search(path)) {
        }

        return best;
    }

    // average cost among possible apples
    Cost evalApple(const State &state) {
        Cost sum = Cost{0.0f, 0.0f};
        int n = State::SIZE - state.length;
        State s = state;

        for (int i = 0; i < n; ++i) {
            sum += evalPath(s);
            s = State::nextApple(s);
        }

        return sum / n;
    }
};

} // namespace Snake

#endif