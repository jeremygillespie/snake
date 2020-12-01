#ifndef APPLE_SEARCH_HPP
#define APPLE_SEARCH_HPP

#include "State.hpp"

namespace Snake {

class AppleSearch {
public:
    virtual bool operator()(Path &result) = 0;
};

// return a different arbitrary path each time
class DepthFirst : public AppleSearch {
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
                unsigned char dir = moves[depth];
                ++moves[depth];

                if (states[depth].canExplore(dir)) {

                    // successor state
                    states[depth + 1] = State::move(states[depth], dir);

                    ++depth;
                    moves[depth] = 0U;

                    if (states[depth].head == states[depth].apple) {
                        // success
                        result = {states[depth],
                                  std::vector<unsigned char>(depth)};

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

private:
    int depth;
    std::array<State, State::SIZE> states;
    std::array<unsigned char, State::SIZE - 1> moves;
};

} // namespace Snake

#endif