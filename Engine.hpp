#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "State.hpp"

namespace Snake {

class Engine {
public:
    Engine(State state);
    virtual Direction nextMove() = 0;
};

} // namespace Snake

#endif