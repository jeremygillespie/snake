#ifndef PATH_HPP
#define PATH_HPP

#include "State.hpp"

namespace Snake {

class Path {
public:
    int time;
    State start, end;
    std::array<State::chunk_type, State::SIZE - 1> route;
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

class Exhaustive : public Pathfinder {
public:
    Exhaustive(const State &start) : Pathfinder{start} {}

    bool search() {}

private:
    std::array<State, State::SIZE - 1> states;
    std::array<State::chunk_type, State::SIZE - 1> dirs;
};

} // namespace Snake

#endif