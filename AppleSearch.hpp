#ifndef APPLE_SEARCH_HPP
#define APPLE_SEARCH_HPP

#include <vector>

#include "State.hpp"

namespace Snake {

namespace AppleSearch {

struct Path {
public:
    State start, end;
    std::vector<State::chunk_type> moves;
};

class Exhaustive {
public:
    // guaranteed complete
    virtual bool operator()(Path &result) = 0;
};

class Partial {
public:
    // guaranteed complexity bound
    virtual bool operator()(Path &result, int attempts) = 0;
};

class DepthFirst : public Exhaustive {
public:
    DepthFirst(const State &start) : depth{0} {
        states[0] = start;
        moves[0] = 0U;
    }

    bool operator()(Path &result) {
        for (;;) {
            if (depth < 0) {
                return false;
            } else if (moves[depth] == 4U) {
                --depth;
            } else {
                State::chunk_type dir = moves[depth];
                ++moves[depth];

                if (states[depth].canExplore(dir)) {

                    // successor state
                    states[depth + 1] = State::move(states[depth], dir);

                    ++depth;
                    moves[depth] = 0U;

                    if (states[depth].head == states[depth].apple) {
                        // success
                        result = {states[0], states[depth],
                                  std::vector<State::chunk_type>(depth)};

                        for (int i = 0; i < depth; ++i) {
                            result.moves[i] = moves[i] - 1U;
                        }

                        --depth;
                        return true;
                    }
                }
            }
        }
    }

    // // success
    // if (states[depth].head == states[depth].apple) {
    //     result = {states[0], states[depth],
    //               std::vector<State::chunk_type>(depth)};
    //     for (int i = 0; i < depth; ++i) {
    //         result.moves[i] = moves[i];
    //     }
    //     return true;
    // }

    // // failure
    // if (depth < 0) {
    //     return false;
    // }

private:
    int depth;
    std::array<State, State::SIZE> states;
    std::array<State::chunk_type, State::SIZE - 1> moves;
}; // namespace AppleSearch

} // namespace AppleSearch

} // namespace Snake

#endif