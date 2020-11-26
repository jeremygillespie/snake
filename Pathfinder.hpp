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
    virtual bool search() = 0;

    virtual bool search(int attempts) = 0;

    Path path;

protected:
    State start;
};

class DepthFirst : public Pathfinder {
public:
    DepthFirst(const State &start) : Pathfinder{start} {}

    bool search() {
        while (true) {

            //  if states[depth].hasSuccessor()
            //      states[depth + 1] = states[depth].successor
            //      ++depth
            //  else
            //      --depth

            if (depth < 0) {
                return false;
            } else if (states[depth].head == states[depth].apple) {
                path = {start, states[depth],
                        std::vector<State::chunk_type>(depth)};
                for (int i = 0; i < depth; ++i) {
                    // set path.route[i] according to states[i]
                }
                return true;
            }
        }
    }

private:
    int depth;
    std::array<State, State::SIZE - 1> states;
};

} // namespace Snake

#endif