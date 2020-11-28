#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "AppleSearch.hpp"
#include "SafetySearch.hpp"

namespace Snake {

struct Cost {
    float time;
    float death;

    bool operator==(const Cost &other) const {
        return death == other.death && time == other.time;
    }

    bool operator<(const Cost &other) const {
        return death == other.death ? time < other.time : death < other.death;
    }

    bool operator>(const Cost &other) const {
        return death == other.death ? time > other.time : death > other.death;
    }

    bool operator<=(const Cost &other) const {
        return death < other.death ||
               (death == other.death && time <= other.time);
    }

    bool operator>=(const Cost &other) const {
        return death > other.death ||
               (death == other.death && time >= other.time);
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
        // TODO
        return AppleSearch::Path{};
    }

private:
    State start;

    Cost evalPath(const State &s) { return Cost{}; }

    Cost evalApple(const State &s) { return Cost{}; }
};

} // namespace Snake

#endif