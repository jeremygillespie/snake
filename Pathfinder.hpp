#ifndef PATH_HPP
#define PATH_HPP

#include <vector>

#include "State.hpp"

namespace Snake {

struct Path {
public:
    State start, end;
    std::vector<State::chunk_type> route;
};

class Pathfinder {
public:
    Pathfinder(const State &start);

    // search until found
    virtual bool search(Path &path) = 0;

    virtual bool search(Path &path, int attempts) = 0;

protected:
    State start;
};

class DepthFirst : public Pathfinder {
public:
    DepthFirst(const State &start) : Pathfinder{start}, depth{0} {
        states[0] = start;
        lastMove[0] = 0U;
    }

    bool search(Path &path) {
        for(;;) {
            for(State::chunk_type dir = lastMove[depth]; dir <= 3U; ++dir)
            {
                if(states[depth].canExplore(dir)) {
                    // successor state
                    lastMove[depth] = dir;
                    states[depth + 1] = State(states[depth], dir);
                    lastMove[depth + 1] = 0U;
                    ++depth;

                    // success
                    if (states[depth].head == states[depth].apple) {
                        path = {start, states[depth],
                                std::vector<State::chunk_type>(depth)};
                        for (int i = 0; i < depth; ++i) {
                            // set path.route[i] according to states[i]
                        }
                        return true;
                    }

                    break;
                }
                // no more successors
                if(dir == 3U) {
                    --depth;

                    // failure
                    if (depth < 0)
                        return false;
                }
            }
        }
    }

private:
    int depth;
    std::array<State, State::SIZE - 1> states;
    std::array<State::chunk_type, State::SIZE - 1> lastMove;
};

} // namespace Snake

#endif