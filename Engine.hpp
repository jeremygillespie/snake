#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "AppleSearch.hpp"
#include "SafetySearch.hpp"

namespace Snake {

class Engine {
public:
    virtual AppleSearch::Path getPath(int nextApple) = 0;
};

class Exhaustive : public Engine {
public:
    Exhaustive(){
        // TODO
    };

    AppleSearch::Path getPath(int nextApple) {
        // TODO
        return AppleSearch::Path{};
    }
};

} // namespace Snake

#endif