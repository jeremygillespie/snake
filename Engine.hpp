#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Pathfinder.hpp"

namespace Snake {

class Engine {
public:
    virtual Path getPath(int nextApple) = 0;
};

class Exhaustive : public Engine {
public:
    Exhaustive(){
        // TODO
    };

    Path getPath(int nextApple) {
        // TODO
        return Path{};
    }
};

} // namespace Snake

#endif